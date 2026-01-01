import os

def remove_bom(file_path):
    with open(file_path, 'rb') as f:
        content = f.read()
    
    if content.startswith(b'\xef\xbb\xbf'):
        content = content[3:]  # 移除BOM标记
        with open(file_path, 'wb') as f:
            f.write(content)
        print(f'已移除BOM标记: {file_path}')
        return True
    else:
        print(f'无BOM标记: {file_path}')
        return False

# 移除model_renderer.cpp的BOM标记
remove_bom('src/model_renderer.cpp')
