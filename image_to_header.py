#!/usr/bin/env python3
import sys
import os
import argparse


def image_to_header(image_path, output_path=None, array_name=None):
    """
    将图片转为 C++ 头文件
    # 基本用法
    python image_to_header.py icon.png

    # 指定输出文件名
    python image_to_header.py icon.png -o resources/icon_data.h

    # 指定数组变量名
    python image_to_header.py icon.png -n my_icon
    """
    # 读取图片二进制
    with open(image_path, 'rb') as f:
        data = f.read()
    
    # 自动生成变量名（从文件名提取）
    if array_name is None:
        base_name = os.path.splitext(os.path.basename(image_path))[0]
        array_name = base_name.replace('-', '_').replace('.', '_').replace(' ', '_')
    
    # 自动生成输出路径
    if output_path is None:
        output_path = array_name + '.h'
    
    # 生成头文件内容
    header_guard = array_name.upper() + '_H'
    
    lines = [
        f'#ifndef {header_guard}',
        f'#define {header_guard}',
        '',
        '#include <cstdint>',
        '',
        f'// Auto-generated from {os.path.basename(image_path)}',
        f'// Size: {len(data)} bytes',
        '',
        f'constexpr unsigned char {array_name}_data[] = {{',
    ]
    
    # 每行 16 个字节，格式化输出
    hex_bytes = [f'0x{b:02x}' for b in data]
    for i in range(0, len(hex_bytes), 16):
        line = '    ' + ', '.join(hex_bytes[i:i+16]) + ','
        lines.append(line)
    
    lines.extend([
        '};',
        '',
        f'constexpr size_t {array_name}_size = sizeof({array_name}_data);',
        '',
        f'#endif // {header_guard}',
        '',
    ])
    
    # 写入文件
    with open(output_path, 'w') as f:
        f.write('\n'.join(lines))
    
    print(f'Generated: {output_path}')
    print(f'  Array: {array_name}_data[{len(data)}]')
    print(f'  Size:  {array_name}_size')


def main():
    parser = argparse.ArgumentParser(description='Convert image to C++ header')
    parser.add_argument('image', help='Input image file')
    parser.add_argument('-o', '--output', help='Output header file')
    parser.add_argument('-n', '--name', help='Array name (default: from filename)')
    
    args = parser.parse_args()
    
    if not os.path.exists(args.image):
        print(f'Error: File not found: {args.image}')
        sys.exit(1)
    
    image_to_header(args.image, args.output, args.name)


if __name__ == '__main__':
    main()