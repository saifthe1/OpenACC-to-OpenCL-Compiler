
AC_INIT(acc2ocl, version-0.1)

AC_PROG_CC

AC_ARG_WITH(boost, [  --with-boost            BOOST path, used by ROSE])
if test "$with_boost" == ""
then
	AC_MSG_ERROR([Need BOOST path for ROSE])
else
	AC_MSG_NOTICE([BOOST path provided])
        AC_SUBST(boost_path, $with_boost)
fi

AC_ARG_WITH(opencl-inc, [  --with-opencl-inc       OpenCL headers path, used by libOpenACC])
if test "$with_opencl_inc" == ""
then
	AC_MSG_ERROR([Need OpenCL headers path])
else
	AC_MSG_NOTICE([OpenCL headers path provided])
        AC_SUBST(opencl_inc_path, $with_opencl_inc)
fi

AC_ARG_WITH(opencl-lib, [  --with-opencl-lib       OpenCL library path, used by libOpenACC])
if test "$with_opencl_lib" == ""
then
	AC_MSG_ERROR([Need OpenCL library path])
else
	AC_MSG_NOTICE([OpenCL library path provided])
        AC_SUBST(opencl_lib_path, $with_opencl_lib)
fi

AC_ARG_WITH(sqlite-inc, [  --with-sqlite-inc       SQLite headers path, used to save evaluation and profiling])
if test "$with_sqlite_inc" == ""
then
	AC_MSG_ERROR([Need SQLite headers path])
else
	AC_MSG_NOTICE([SQLite headers path provided])
        AC_SUBST(sqlite_inc_path, $with_sqlite_inc)
fi

AC_ARG_WITH(sqlite-lib, [  --with-sqlite-lib       SQLite library path, used to save evaluation and profiling])
if test "$with_sqlite_lib" == ""
then
	AC_MSG_ERROR([Need SQLite library path])
else
	AC_MSG_NOTICE([SQLite library path provided])
        AC_SUBST(sqlite_lib_path, $with_sqlite_lib)
fi

AC_OUTPUT(libopenacc/lib/Makefile)
AC_OUTPUT(tests/001/Makefile)
AC_OUTPUT(tests/101/Makefile)
AC_OUTPUT(tests/102/Makefile)
AC_OUTPUT(tests/103/Makefile)
AC_OUTPUT(tests/501/Makefile)
AC_OUTPUT(tests/scripts/100-rules.mk)
AC_OUTPUT(tests/scripts/environment.mk)
AC_OUTPUT(benchmark/generate.sh)
AC_OUTPUT(benchmark/compile.sh)
AC_OUTPUT(benchmark/execute.sh)
AC_OUTPUT(benchmark/execute_min.sh)
AC_OUTPUT(benchmark/process.sh)
AC_OUTPUT(benchmark/config.rc)
AC_OUTPUT(rose.sh)

chmod +x rose.sh
chmod +x benchmark/execute_min.sh benchmark/execute.sh benchmark/generate.sh benchmark/compile.sh benchmark/process.sh
