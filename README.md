### 使用即时模式 GUI Nuklear 第三方库编写的 EasyX 安装程序

### nuklear_easyx.h 是 Nuklear 基于 EasyX 图形窗口的实现

### EasyX 对各版本 Mingw64 的支持情况：
	- x86_64-8.5.0-release-posix-seh-rt_v10-rev0
	- x86_64-8.5.0-release-win32-seh-rt_v10-rev0
	- x86_64-12.2.0-release-win32-seh-msvcrt-rt_v10-rev2
	- x86_64-12.2.0-release-posix-seh-msvcrt-rt_v10-rev2
	- x86_64-13.2.0-release-posix-seh-msvcrt-rt_v11-rev1
	- x86_64-13.2.0-release-win32-seh-msvcrt-rt_v11-rev1
	- x86_64-14.2.0-release-win32-seh-msvcrt-rt_v12-rev2
	- x86_64-14.2.0-release-posix-seh-msvcrt-rt_v12-rev2
	- x86_64-15.1.0-release-posix-seh-msvcrt-rt_v12-rev0
	- x86_64-15.1.0-release-win32-seh-msvcrt-rt_v12-rev0
	- x86_64-15.2.0-release-posix-seh-msvcrt-rt_v13-rev0
	- x86_64-15.2.0-release-win32-seh-msvcrt-rt_v13-rev0
	- ...
	- 总结：64位、seh、msvcrt 一定支持

	- i686-8.5.0-release-posix-sjlj-rt_v10-rev0
	- i686-8.5.0-release-win32-sjlj-rt_v10-rev0
	- i686-11.2.0-release-posix-sjlj-rt_v9-rev1
	- ...
	- 总结：(x32|i686)、sjlj、msvcrt 一定支持

### 以下需要使用 /lib-for-devcpp_5.4.0/ 下的 libeasyx.a
	- x32-4.7.2-release-posix-dwarf-rev10
	- x32-4.7.2-release-win32-dwarf-rev3
	- x32-4.8.1-release-win32-dwarf-rev0
	- i686-8.1.0-release-posix-dwarf-rt_v6-rev0
	- i686-8.5.0-release-win32-dwarf-rt_v10-rev0
	- i686-11.2.0-release-posix-dwarf-rt_v9-rev1
	- i686-11.2.0-release-win32-dwarf-rt_v9-rev1
	- i686-12.2.0-release-posix-dwarf-msvcrt-rt_v10-rev2
	- i686-14.2.0-release-posix-dwarf-msvcrt-rt_v12-rev0
	- i686-15.2.0-release-posix-dwarf-msvcrt-rt_v13-rev0
	- ... 
	- 总结： (x32|i686)、dwarf、msvcrt 一定支持


#### 不支持版本

	- i686-14.2.0-release-win32-dwarf-ucrt-rt_v12-rev0
	- i686-15.2.0-release-mcf-dwarf-ucrt-rt_v13-rev0

	- x64-4.7.2-release-win32-sjlj-rev10
	- x86_64-15.2.0-release-mcf-seh-ucrt-rt_v13-rev0
	- ....
	- 总结：ucrt 一定不支持