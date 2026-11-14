import os
import subprocess
import sys

project_dir = r"C:\Users\Nuar\Downloads\tren_stm (1)\tren_stm"
bin_dir = r"C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\14.2 rel1\bin"
gcc = os.path.join(bin_dir, "arm-none-eabi-gcc.exe")
objcopy = os.path.join(bin_dir, "arm-none-eabi-objcopy.exe")
size_tool = os.path.join(bin_dir, "arm-none-eabi-size.exe")

out_dir = os.path.join(project_dir, "Debug")
os.makedirs(out_dir, exist_ok=True)

includes = [
    "-I" + os.path.join(project_dir, "Core", "Inc"),
    "-I" + os.path.join(project_dir, "Drivers", "STM32F4xx_HAL_Driver", "Inc"),
    "-I" + os.path.join(project_dir, "Drivers", "STM32F4xx_HAL_Driver", "Inc", "Legacy"),
    "-I" + os.path.join(project_dir, "Drivers", "CMSIS", "Device", "ST", "STM32F4xx", "Include"),
    "-I" + os.path.join(project_dir, "Drivers", "CMSIS", "Include"),
]

c_flags = [
    "-mcpu=cortex-m4",
    "-std=gnu11",
    "-g3",
    "-DDEBUG",
    "-DUSE_HAL_DRIVER",
    "-DSTM32F401xC",
    "-O0",
    "-ffunction-sections",
    "-fdata-sections",
    "-Wall",
    "--specs=nano.specs",
    "-mfpu=fpv4-sp-d16",
    "-mfloat-abi=hard",
    "-mthumb",
]

s_flags = [
    "-mcpu=cortex-m4",
    "-g3",
    "-DDEBUG",
    "-x", "assembler-with-cpp",
    "--specs=nano.specs",
    "-mfpu=fpv4-sp-d16",
    "-mfloat-abi=hard",
    "-mthumb",
]

linker_script = os.path.join(project_dir, "STM32F401CCUX_FLASH.ld")

c_sources = []
# Core/Src
core_src = os.path.join(project_dir, "Core", "Src")
for f in os.listdir(core_src):
    if f.endswith(".c"):
        c_sources.append(os.path.join(core_src, f))

# Drivers/STM32F4xx_HAL_Driver/Src
hal_src = os.path.join(project_dir, "Drivers", "STM32F4xx_HAL_Driver", "Src")
for f in os.listdir(hal_src):
    if f.endswith(".c"):
        c_sources.append(os.path.join(hal_src, f))

# Assembly startup
s_sources = [os.path.join(project_dir, "Core", "Startup", "startup_stm32f401ccux.s")]

objs = []

print(f"Compiling {len(s_sources)} assembly sources...")
for s in s_sources:
    rel = os.path.relpath(s, project_dir)
    obj = os.path.join(out_dir, os.path.splitext(rel)[0] + ".o")
    os.makedirs(os.path.dirname(obj), exist_ok=True)
    cmd = [gcc] + s_flags + ["-c", s, "-o", obj]
    res = subprocess.run(cmd, capture_output=True, text=True)
    if res.returncode != 0:
        print(f"Error compiling {s}:\n{res.stderr}")
        sys.exit(1)
    objs.append(obj)

print(f"Compiling {len(c_sources)} C sources...")
for c in c_sources:
    rel = os.path.relpath(c, project_dir)
    obj = os.path.join(out_dir, os.path.splitext(rel)[0] + ".o")
    os.makedirs(os.path.dirname(obj), exist_ok=True)
    cmd = [gcc] + c_flags + includes + ["-c", c, "-o", obj]
    res = subprocess.run(cmd, capture_output=True, text=True)
    if res.returncode != 0:
        print(f"Error compiling {c}:\n{res.stderr}")
        sys.exit(1)
    objs.append(obj)

elf_path = os.path.join(out_dir, "tren_stm.elf")
map_path = os.path.join(out_dir, "tren_stm.map")
hex_path = os.path.join(out_dir, "tren_stm.hex")
bin_path = os.path.join(out_dir, "tren_stm.bin")

link_cmd = [
    gcc,
    "-o", elf_path,
] + objs + [
    "-mcpu=cortex-m4",
    f"-T{linker_script}",
    "--specs=nosys.specs",
    f"-Wl,-Map={map_path}",
    "-Wl,--gc-sections",
    "-static",
    "--specs=nano.specs",
    "-mfpu=fpv4-sp-d16",
    "-mfloat-abi=hard",
    "-mthumb",
    "-Wl,--start-group",
    "-lc",
    "-lm",
    "-Wl,--end-group",
]

print("Linking...")
res = subprocess.run(link_cmd, capture_output=True, text=True)
if res.returncode != 0:
    print(f"Link error:\n{res.stderr}")
    sys.exit(1)

print(f"Generating HEX: {hex_path}...")
hex_cmd = [objcopy, "-O", "ihex", elf_path, hex_path]
res = subprocess.run(hex_cmd, capture_output=True, text=True)
if res.returncode != 0:
    print(f"Hex error:\n{res.stderr}")
    sys.exit(1)

print(f"Generating BIN: {bin_path}...")
bin_cmd = [objcopy, "-O", "binary", "-S", elf_path, bin_path]
res = subprocess.run(bin_cmd, capture_output=True, text=True)
if res.returncode != 0:
    print(f"Bin error:\n{res.stderr}")
    sys.exit(1)

print("Size:")
subprocess.run([size_tool, elf_path])

print("BUILD SUCCESSFUL!")
