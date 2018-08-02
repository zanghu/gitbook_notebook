// 该头文件一般位于 ananconda3/include下, 对应的动态库
#include <python3.6m/Python.h>
#include <iostream>
 
using namespace std;

int add(int arg1, int arg2)
{
    return arg1 + arg2;
}
 
int sub(int arg1, int arg2)
{
    return arg1 - arg2;
}
 
static PyObject* math_add(PyObject* self, PyObject* args)
{
    int arg1, arg2;
    if (!PyArg_ParseTuple(args, "ii", &arg1, &arg2)) {
        return NULL;
    }
    int result = add(arg1, arg2);
    return (PyObject*)Py_BuildValue("i", result);
}
 
static PyObject* math_sub(PyObject* self, PyObject* args)
{
    int arg1, arg2;
    if (!PyArg_ParseTuple(args, "ii", &arg1, &arg2)) {
        return NULL;
    }
    int result = sub(arg1, arg2);
    return (PyObject*)Py_BuildValue("i", result);
}
 
static PyMethodDef MathMethods[] = {
    { "add", math_add, METH_VARARGS, "Execute math command:add." },
    { "sub", math_sub, METH_VARARGS, "Execute math command:sub." },
    { NULL, NULL }
};
 
static struct PyModuleDef MathModule =
{
    PyModuleDef_HEAD_INIT,
    "cmathapi", // 对应于“模块名.__name__”, 这里的名字似乎其实不重要, 填写其他字符串(与模块名称不一致)也不会影响编译好的动态库在使用时的模块名称
    "example module doc string", // 对应于“模块名.__doc__”, 也不是一个很重要的字段
    -1,
    MathMethods,
    NULL,
    NULL,
    NULL,
    NULL
};
 
static PyObject* __mathError;

// 这里的函数名称似乎非常重要, 必须采用“pyInit_模块名”的模式命名, 斗则无法从编译得到的动态库中import指定模块名称
PyMODINIT_FUNC PyInit_cmathapi()
{
    PyObject* module = PyModule_Create(&MathModule);
    if (NULL == module) {
        return NULL;
    }
    __mathError = PyErr_NewException("math.error", NULL, NULL);
    Py_INCREF(__mathError);
    PyModule_AddObject(module, "error", __mathError);
    return module;
}

 /**
// 调用Add函数,传两个int型参数
void Add()
{
        Py_Initialize();
        PyObject* pModule = NULL;
        PyObject* pFunc = NULL;
        pModule = PyImport_ImportModule("cmathapi");    // Test:Python文件名
        pFunc = PyObject_GetAttrString(pModule, "add");   // Add:Python文件中的函数名
        Py_DECREF(pModule);

        // 创建参数
        PyObject* pArgs = PyTuple_New(2);                 // 函数调用的参数传递均是以元组的形式打包的,2表示参数个数
        PyTuple_SetItem(pArgs, 0, Py_BuildValue("i", 5)); // 0---序号  i表示创建int型变量 
        PyTuple_SetItem(pArgs, 1, Py_BuildValue("i", 7)); // 1---序号

        // 返回值
        PyObject* pReturn = NULL;
        pReturn = PyEval_CallObject(pFunc, pArgs);        // 调用函数

        // 将返回值转换为int类型.
        int result;
        PyArg_Parse(pReturn, "i", &result);               // i表示转换成int型变量
        cout << "5+7 = " << result << endl;
        Py_Finalize(); 
}

 
int main(int argc, char *argv[])   
{
    PyImport_AppendInittab("cmathapi", PyInit_cmathapi);
    //Py_Initialize();

    // 传递argv[0]给Python解释器  
    //Py_SetProgramName(argv[0]);   
     
    cout << "Start Test==========" << endl;
    cout << "Add()--------------------" << endl;
    Add();
    cout << "Finish Test==========" << endl;

    return 0;
}
*/