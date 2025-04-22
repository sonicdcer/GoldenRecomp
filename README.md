Building instructions:

```bash
git clone https://github.com/kholdfuzion/Goldenrecomp --recurse-submodules
```

* place ge007.us.tlbfree.z64 on the root of the repo.

```bash
.\N64Recomp.exe us.toml
.\N64Recomp.exe us.toml --dump-context
.\RSPRecomp.exe aspMain.toml
```

place this function inside repo\lib\N64ModernRuntime\librecomp\src\pi.cpp

```c
extern "C" void boot_osPiRawStartDma(uint8_t* rdram, recomp_context* ctx) {
    uint32_t direction = ctx->r4;
    uint32_t device_address = ctx->r5;
    gpr rdram_address = ctx->r6;
    uint32_t size = ctx->r7;

    assert(direction == 0); // Only reads

    // Complete the DMA synchronously (the game immediately waits until it's done anyways)
    recomp::do_rom_read(rdram, rdram_address, device_address + recomp::rom_base, size);
}```

* Open project folder with visual studio
* Select the GoldenRecomp.exe target
* Set either x64-Debug or z64-ReleaseWithDebInfo as required
* Build
