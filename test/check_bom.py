import os

files = ['include/quickjs_engine.h', 'include/pointcloud.h', 'include/model_loader_js.h', 'include/gl_extensions.h', 'include/utils.h', 'include/camera.h', 'include/viewer.h']
for f in files:
    if os.path.exists(f):
        with open(f, 'rb') as file:
            content = file.read()
            has_bom = content.startswith(b'\xef\xbb\xbf')
            print(f'{f}: {"BOM found" if has_bom else "No BOM"}')
    else:
        print(f'{f}: File not found')
