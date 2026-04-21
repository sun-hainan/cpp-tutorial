# -*- coding: utf-8 -*-
import codecs

path = r'D:\openclaw-home\.openclaw\workspace\cpp-tutorial\05_strings_arrays.cpp'
with open(path, 'rb') as f:
    raw = f.read()

old = b'    q5_sorting();\n    q6_binary_search();\n    q7_pointer_array();\n\n    std::cout << "\\n[\xe6\xa8\xa1\xe5\x9d\x97\xe4\xba\x94\xe5\xae\x8c\xe6\x88\x90]" << std::endl;\n    return 0;\n}\n'

new_text = """
    q5_sorting();
    q6_binary_search();
    q7_pointer_array();

    std::cout << "\\n[\xe6\xa8\xa1\xe5\x9d\x97\xe4\xba\x94\xe5\xae\x8c\xe6\x88\x90]" << std::endl;
    return 0;
}

// ================================================================
// 【练习题】
// ================================================================
// 1. 用 std::string 实现一个字符串反转函数，并对比测试和手写的性能差异
// 2. 实现一个二维数组的赋值转换、转换、查询和查找
// 3. 编写一个二分查找函数，对比线性查找和二分查找的时间复杂度
// 4. 用 std::vector 实现一个快速排序，并创建一个自定义排序的比较函数
// 5. 实现一个基于二分查找的选择习题，并对对比全零查找和排序的性能
""".encode('utf-8')

if old in raw:
    raw = raw.replace(old, new_text, 1)
    with open(path, 'wb') as f:
        f.write(raw)
    print('OK')
else:
    print('ERROR')
    idx = raw.rfind(b'return 0;')
    print(repr(raw[idx-100:idx+100]))
