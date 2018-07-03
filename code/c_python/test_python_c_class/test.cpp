/* test.cpp */
// 该头文件一般位于 ananconda3/include下, 对应的动态库
#include <python3.6m/Python.h>
#include <python3.6m/structmember.h> 
#include <iostream>
#include <sstream>

using namespace std;


/* 第零部分：定义C结构体 */

// Python没有C++接口, 所以定义为struct而不是class
typedef struct _CScore
{
    PyObject_HEAD      // == PyObject ob_base;  定义一个PyObject对象.
    char*  m_szName;
    float  m_dMath;
    float  m_dEnglish;
}CScore;


/* 第一部分：包装C/C++类方法 */

//////////////////////////////////////////////////////////////
// CScore类的所有内置、构造方法.
//
static void CScore_init(CScore* Self, PyObject* pArgs)    //构造方法.
{
    const char* Name = 0;
    if(!PyArg_ParseTuple(pArgs, "sff", &Name, &Self->m_dMath, &Self->m_dEnglish)) {
        cout << "Parse the argument FAILED! You should pass correct values!" << endl;
        return ;
    }

    Self->m_szName = new char[strlen(Name) + 1];
    strcpy(Self->m_szName, Name);
}

static void CScore_Destruct(CScore* Self)                   //析构方法.
{
    if(Self->m_szName)
        delete [] Self->m_szName;              //先释放其字符指针对象.

    //如果还有PyObject*成员的话，要一并释放之.
    //如：Py_XDECREF(Self->Member);
    Py_TYPE(Self)->tp_free((PyObject*)Self);      //释放对象/实例.
}

static PyObject* CScore_Str(CScore* Self)             //调用str/print时自动调用此函数.
{
    ostringstream OStr;
    OStr << "Name    : " << Self->m_szName << endl
           << "Math    : " << Self->m_dMath << endl
           << "English : " << Self->m_dEnglish << endl;
    string Str = OStr.str();
    return Py_BuildValue("s", Str.c_str());
}

static PyObject* CScore_Repr(CScore* Self)            //调用repr内置函数时自动调用.
{
    return CScore_Str(Self);
}

////////////////////////////////////////////////////////////
// CScore类的所有Get方法.
//
static PyObject* CScore_GetName(CScore* Self)
{
    return Py_BuildValue("s", Self->m_szName);
}

static PyObject* CScore_GetMath(CScore* Self)
{
    return Py_BuildValue("f", Self->m_dMath);
}

static PyObject* CScore_GetEnglish(CScore* Self)
{
    return Py_BuildValue("f", Self->m_dEnglish);
}

////////////////////////////////////////////////////////////
// CScore类的所有Set方法.
//
static PyObject* CScore_SetMath(CScore* Self, PyObject* Argvs)
{
    Py_INCREF(Py_None);
    if(!PyArg_ParseTuple(Argvs, "f", &Self->m_dMath)) {
        cout << "Parse the argument FAILED! You should pass correct values!" << endl;
        return Py_None;
    }

    return Py_None;
}

static PyObject* CScore_SetEnglish(CScore* Self, PyObject* Argvs)
{
    Py_INCREF(Py_None);
    if(!PyArg_ParseTuple(Argvs, "f", &Self->m_dEnglish)) {
        cout << "Parse the argument FAILED! You should pass correct values!" << endl;
        return Py_None;
    }

    return Py_None;
}

////////////////////////////////////////////////////////////
// CScore类的其他方法, 计算分数总和
//
static PyObject* CScore_CalTotalScore(CScore* Self)
{
    cout << "The scores as follows:" << endl
        << "==============================" << endl
        << "Name    : " << Self->m_szName << endl
        << "Math    : " << Self->m_dMath << endl
        << "English : " << Self->m_dEnglish << endl
        << "Total   : " << Self->m_dMath + Self->m_dEnglish << endl
        << "==============================" << endl;

    Py_XINCREF(Py_None);
    return Py_None;
}


/* 第二部分：定义Python类成员 */
static PyMemberDef CScore_DataMembers[] =         //类/结构的数据成员的说明.
{
    // 注意: 下面的每一行的第一个参数和最后一个参数必须强制转化为(char *), 否则编译报警(但不会报错 ), 使用编译出的库运行测试用例会失败
    {(char *)"m_szName",   T_STRING, offsetof(CScore, m_szName),   READONLY, (char *)"The Name of instance"},
    {(char *)"m_dMath",    T_FLOAT,  offsetof(CScore, m_dMath),    READONLY, (char *)"The Math score of instance."},
    {(char *)"m_dEnglish", T_FLOAT,  offsetof(CScore, m_dEnglish), READONLY, (char *)"The English score of instance."},

    //{NULL, NULL, NULL, 0, NULL}
    {NULL}
};


/* 第三部分：定义Python类方法 */
static PyMethodDef CScore_MethodMembers[] =      //类的所有成员函数结构列表.
{
    {"GetName",    (PyCFunction)CScore_GetName, METH_NOARGS,     "Get the name of instance."},
    {"GetMath",    (PyCFunction)CScore_GetMath, METH_NOARGS,     "Get the math score of instance."},
    {"GetEnglish", (PyCFunction)CScore_GetEnglish, METH_NOARGS,  "Get the english score of isntance."},

    {"SetMath",    (PyCFunction)CScore_SetMath, METH_VARARGS,    "Set the math score of instance."},
    {"SetEnglish", (PyCFunction)CScore_SetEnglish, METH_VARARGS, "Set the english of instance."},

    {"CalTotalScore",  (PyCFunction)CScore_CalTotalScore, METH_NOARGS,   "Print the total score and all information of instance."},

    //{NULL, NULL, NULL, NULL}
    {NULL, NULL}
};


/* 第四部分：配置Python类定义 */

////////////////////////////////////////////////////////////
// 类/结构的所有成员、内置属性的说明信息.
//
static PyTypeObject CScore_ClassInfo =
{
    PyVarObject_HEAD_INIT(NULL, 0)"Module.MyCppClass",                 //可以通过__class__获得这个字符串. CPP可以用类.__name__获取.
    sizeof(CScore),                 //类/结构的长度.调用PyObject_New时需要知道其大小.
    0,
    (destructor)CScore_Destruct,    //类的析构函数.
    0,
    0,
    0,
    0,
    (reprfunc)CScore_Repr,          //repr 内置函数调用。
    0,
    0,
    0,
    0,
    0,
    (reprfunc)CScore_Str,          //Str/print内置函数调用.
    0,
    0,
    0,
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,                 //如果没有提供方法的话，为Py_TPFLAGS_DEFAULE
    "MyCppClass Objects---Extensioned by C++!",                   //__doc__,类/结构的DocString.
    0,
    0,
    0,
    0,
    0,
    0,
    CScore_MethodMembers,        //类的所有方法集合.
    CScore_DataMembers,          //类的所有数据成员集合.
    0,
    0,
    0,
    0,
    0,
    0,
    (initproc)CScore_init,      //类的构造函数.
    0,
};


/* 附加部分：定义Python模块 */
////////////////////////////////////////////////////////////
// 此模块的说明信息.
//
static PyModuleDef ModuleInfo =
{
    PyModuleDef_HEAD_INIT,
    "My C++ Class Module",               //模块的内置名--__name__.
    "This Module Created By C++--extension a class to Python!",                 //模块的DocString.__doc__
    -1,
    NULL, NULL, NULL, NULL, NULL
};


/* 第五部分：导出Python模块 + 将类加入模块 */

////////////////////////////////////////////////////////////
// 模块的初始化函数. import 时自动调用.
//
PyMODINIT_FUNC           // == __decslpec(dllexport) PyObject*, 定义导出函数.
PyInit_cscore(void)       //模块外部名称为--CppClass
{
    PyObject* pReturn = 0;
    CScore_ClassInfo.tp_new = PyType_GenericNew;       // 此类的new内置函数—建立对象.

    /////////////////////////////////////////////////////
    // 完成对象类型的初始化—包括添加其继承特性等等。
    // 如果成功，则返回0，否则返回-1并抛出异常.
    //

    if(PyType_Ready(&CScore_ClassInfo) < 0) {
        cout << "error 1" << endl;
        return NULL;
    }

    pReturn = PyModule_Create(&ModuleInfo);          // 根据模块信息创建模块.
    if(pReturn == 0) {
        cout << "error 1" << endl;
        return NULL;
    }

    Py_INCREF(&ModuleInfo);
    PyModule_AddObject(pReturn, "CScore", (PyObject*)&CScore_ClassInfo); // 将这个类加入到模块的Dictionary中.

    return pReturn;
}