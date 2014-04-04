// std::cout のためにC++標準ライブラリー iostream を使用。
#include <iostream>
// std::runtime_error のためにC++標準ライブラリー stdexcept を使用。
#include <stdexcept>

// Pythonバインディングのために /usr/include/python3.3/Ptyhon.h を使用。
#include <Python.h>

// C++の無名名前空間で定義するよ
namespace
{

	// Pythonから呼び出せる関数のテスト test1
	//   - Pythonからの引数: なし
	//   - Pythonへのreturn: Python文字列型（Py_BuildValueで生成）
	// C++ では
	//   - 無名名前空間（またはstatic）の 関数として
	//   - シンボル名 test1 を
	//   - return型を auto -> PyObject* 型 として
	//     - （C++では `int f() { }` と `auto f() -> int { }` は全く同じ意味の異なる記述です。）
	//   - PyObject* const 型の引数2つをシグニチャーとしつつも
	//     - 内部で使用はしないので仮引数に名前は付けず
	//   - 関数本体の定義としては、
	//     - std::cout で標準出力へ文字列を出力し、
	//     - return でPythonの文字列型で文字列を出力
	// している関数定義、といったところ。
	auto test1
		(PyObject* const
		, PyObject* const
		)
		-> PyObject*
	{
		// C++ モジュールレベルでの動作確認のため適当に標準出力。
		std::cout << "*** test1 called ***" << std::endl;

		// Py_BuildValue を使うとPythonの型の値を作れる
		// to see: https://docs.python.org/3.3/extending/extending.html#building-arbitrary-values
		return Py_BuildValue("s", "hello Python3-FFI-C++");
	}

	// Pythonから使用できるC関数の定義を列挙します。
	//   - { pythonで名乗る関数名, C関数, 引数オプション, 説明文 }
	//   - 列挙する関数は今回は1つですが複数定義できます。
	//   - 最後の { nullptr, nullptr, 0, nullptr } はこの機能の実装詳細が「番兵法」を用いる為に必要な番兵さんです。
	PyMethodDef test_methods[] =
	{ { "test1", test1, METH_NOARGS, "std::cout and return test string" }
	, { nullptr, nullptr, 0, nullptr }
	};

	// Pythonからモジュールとして認識される為の自己紹介を定義。
	struct PyModuleDef test_module_definition =
	{ PyModuleDef_HEAD_INIT
	, "test"
	, nullptr
	, -1
	, test_methods
	};

	// Python3がモジュールを初期化するために呼び出す関数を定義。
	//   #define PyMODINIT_FUNC extern "C" PyObject* が用意されているので、
	//   PyMODINIT_FUNC PyInit_test() としても良いが、
	//   auto f() -> return_type の記述法に統一したければ次の実装の様に書ける
	extern "C" auto PyInit_test()
		-> PyObject*
	{
		auto module = PyModule_Create(&test_module_definition);

		if (module == nullptr)
			throw std::runtime_error("fatal: cannot init module.");

		return module;
	}

}