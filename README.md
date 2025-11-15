# ARM Cortex-M4 (QEMU)

Small bare-metal C project for Cortex-M4. Runs in QEMU

Build and run:

to start QEMU 
```
make run
```
can debug using gdb ( vscode project settings works )


# Environmet 
.vscode settings to launch debug and start QEMU
debug uses Cortex-Debug vscode extension.

make sure that `arm-none-eabi-gdb` is configured in cortex-debug settings.

example 
``` json
  "cortex-debug.armToolchainPath": "/opt/homebrew/bin",
  "cortex-debug.gdbPath": "/opt/homebrew/bin/arm-none-eabi-gdb",
  "cortex-debug.armToolchainPrefix": "arm-none-eabi",
  "cortex-debug.openocdPath": "/opt/homebrew/bin/openocd"
```


