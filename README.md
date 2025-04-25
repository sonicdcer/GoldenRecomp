Building instructions:

```bash
git clone https://github.com/kholdfuzion/Goldenrecomp --recurse-submodules
```

* place ge007.tlbfree.elf and ge007.tlbfree.z64 on the root of the repo.

```bash
.\N64Recomp.exe us.toml
.\N64Recomp.exe us.toml --dump-context
.\RSPRecomp.exe aspMain.toml
```

* Open project folder with visual studio
* Select the GoldenRecomp.exe target
* Set either x64-Debug or z64-ReleaseWithDebInfo as required
* Build
