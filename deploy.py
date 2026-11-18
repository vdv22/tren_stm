import shutil, os, zipfile

build_dir = r"C:\Users\Nuar\Downloads\tren_stm (1)\tren_stm\Debug"
down_dir = r"C:\Users\Nuar\Downloads"

hex_src = os.path.join(build_dir, "tren_stm.hex")
bin_src = os.path.join(build_dir, "tren_stm.bin")

shutil.copyfile(hex_src, os.path.join(down_dir, "tren_stm.hex"))
shutil.copyfile(hex_src, os.path.join(down_dir, "tren_stm_FINAL.hex"))
shutil.copyfile(bin_src, os.path.join(down_dir, "tren_stm.bin"))

zip_path = os.path.join(down_dir, "tren_stm_ready.zip")
with zipfile.ZipFile(zip_path, "w", zipfile.ZIP_DEFLATED) as z:
    z.write(hex_src, "tren_stm.hex")
    z.write(hex_src, "tren_stm_FINAL.hex")
    z.write(bin_src, "tren_stm.bin")

print("Deliverables successfully updated in Downloads!")
for name in ["tren_stm.hex", "tren_stm_FINAL.hex", "tren_stm.bin", "tren_stm_ready.zip"]:
    p = os.path.join(down_dir, name)
    print(f"  {name}: {os.path.getsize(p)} bytes, mtime: {os.path.getmtime(p)}")
