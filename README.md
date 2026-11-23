# RISC-V Educational Kernel

A small multithreaded operating system kernel for the **RISC-V 64-bit (RV64IMA)** architecture, written in C++ and RISC-V assembly. It runs bare-metal on the QEMU `virt` machine.

It was written as the Operating Systems 1 course project. It is built on the course skeleton, which is derived from [xv6](https://github.com/mit-pdos/xv6-riscv), and provides:

- a custom heap memory allocator
- threads with synchronous context switching
- kernel semaphores
- console I/O
- a layered **C API** and **C++ API** that reach the kernel through system calls (`ecall`)

## Features

### Memory allocator
[`src/memoryAllocator.cpp`](src/memoryAllocator.cpp)
- A singleton allocator that manages the heap between `HEAP_START_ADDR` and `HEAP_END_ADDR`.
- It uses a **first-fit** strategy over a linked list of segments, with sizes aligned to `MEM_BLOCK_SIZE`.
- Freeing a block **merges it with free neighbouring segments** to reduce fragmentation.
- It can report the total free space and the size of the largest free block.

### Threads
[`src/pcb.cpp`](src/pcb.cpp), [`src/scheduler.cpp`](src/scheduler.cpp), [`src/contextSwitch.S`](src/contextSwitch.S)
- Each thread is described by a PCB (Process Control Block) that holds its own stack, context (`ra`, `sp`), state flags and ID.
- The scheduler is a **FIFO ready queue**.
- Context switches are synchronous, through `thread_dispatch()`. A thread also gives up the CPU when it blocks on a semaphore or exits.
- Threads start through a wrapper that switches to the right privilege mode (`sret`). When the thread body returns, the wrapper calls `thread_exit()`.

### Semaphores
[`src/Semaphore.cpp`](src/Semaphore.cpp)
- Classic counting semaphores, each with a FIFO queue of blocked threads.
- They support `wait`, `signal` and `close`. Closing a semaphore unblocks every waiting thread, and their `wait` call returns an error.

### Trap handling and system calls
[`src/supervisorTrap.S`](src/supervisorTrap.S), [`src/riscv.cpp`](src/riscv.cpp)
- The supervisor trap vector saves all 32 registers and passes control to a C++ handler.
- The handler deals with:
  - `ecall` from U-mode and S-mode (system calls)
  - supervisor software interrupts (timer)
  - external interrupts (console)
- The system call number goes in `a0` and the arguments in `a1`–`a4`. The result is returned in `a0`.

## System call API

### C API ([`h/syscall_c.hpp`](h/syscall_c.hpp))

| Code   | Function                                   | Description                         |
|--------|--------------------------------------------|-------------------------------------|
| `0x01` | `void* mem_alloc(size_t size)`             | Allocate memory                     |
| `0x02` | `int mem_free(void* ptr)`                  | Free memory                         |
| `0x03` | `size_t mem_get_free_space()`              | Total free heap space               |
| `0x04` | `size_t mem_get_largest_free_block()`      | Size of the largest free block      |
| `0x11` | `int thread_create(thread_t*, void(*)(void*), void*)` | Create and start a thread |
| `0x12` | `int thread_exit()`                        | Terminate the running thread        |
| `0x13` | `void thread_dispatch()`                   | Yield the CPU                       |
| `0x21` | `int sem_open(sem_t*, unsigned init)`      | Create a semaphore                  |
| `0x22` | `int sem_close(sem_t)`                     | Close a semaphore                   |
| `0x23` | `int sem_wait(sem_t)`                      | Wait (P operation)                  |
| `0x24` | `int sem_signal(sem_t)`                    | Signal (V operation)                |
| `0x41` | `char getc()`                              | Read a character from the console   |
| `0x42` | `void putc(char)`                          | Write a character to the console    |

### C++ API ([`h/syscall_cpp.hpp`](h/syscall_cpp.hpp))

The C++ API wraps the C API in object-oriented classes:

- `Thread`: create a thread from a function pointer, or subclass it and override `run()`. It provides `start()` and `dispatch()`.
- `Semaphore`: an RAII wrapper with `wait()` and `signal()`.
- `Console`: `getc()` and `putc()`.
- Global `operator new` and `operator delete` are routed to `mem_alloc` and `mem_free`.

## Project structure

```
.
├── h/            Kernel headers (PCB, scheduler, allocator, semaphores, syscalls, RISC-V CSR helpers)
├── src/          Kernel implementation (C++ and RISC-V assembly)
├── test/         Public course tests (threads, producer–consumer, system mode)
├── lib/          Provided hardware and console libraries (hw.lib, console.lib)
├── kernel.ld     Linker script
└── Makefile      Build and run targets
```

## Building and running

### Prerequisites
- A RISC-V GNU toolchain: `riscv64-unknown-elf-`, `riscv64-linux-gnu-` or `riscv64-unknown-linux-gnu-`
- `qemu-system-riscv64`
- GNU Make
- A Linux environment (the course VM, WSL or native Linux)

### Commands
```bash
make            # build the kernel image (kernel) and its disassembly (kernel.asm)
make qemu       # build and run in QEMU (-machine virt, 128 MB RAM, 1 core)
make qemu-gdb   # run in QEMU and wait for a GDB connection
make clean      # remove build artifacts
```

The Makefile detects the toolchain prefix automatically. To set it yourself, run, for example, `make TOOLPREFIX=riscv64-linux-gnu-`.

To exit QEMU, press `Ctrl+A`, then `X`.

## Running the tests

On startup the kernel runs `userMain()`, which asks you to pick a test:

```
Unesite broj testa? [1-7]
```

Type the test number and press Enter.

| Test | Description                                                   | Status  |
|------|---------------------------------------------------------------|---------|
| 1    | Threads, C API, synchronous context switch                    | ✅      |
| 2    | Threads, C++ API, synchronous context switch                  | ✅      |
| 3    | Producer–consumer, full C API with semaphores                 | ✅      |
| 4    | Producer–consumer, full C++ API with semaphores               | ✅      |
| 5    | `time_sleep`, C API                                           | ❌ Not implemented |
| 6    | C++ API with asynchronous (preemptive) context switch         | ❌ Not implemented |
| 7    | Checks that user code runs in user mode (expected not to end normally) | ✅ |

You choose which tests are compiled in with the `LEVEL_N_IMPLEMENTED` macros in [`test/userMain.cpp`](test/userMain.cpp).

## Limitations
- **No preemption:** the timer interrupt is acknowledged but does not trigger a time-slice context switch, so threads switch only when they yield, block or exit.
- **No `time_sleep`:** it is declared in the API but not implemented.
- `PeriodicThread` is not implemented.
- Single core only (`CPU_CORE_COUNT = 1`).

## License
The course skeleton is based on xv6, © MIT, released under the MIT license. See [LICENSE](LICENSE).
