###############################################################################
# 通用makefile,默认输出test,如果输出文件名以xxx.a格式,则编译生成静态库
#
# Created on: 2016年12月17日
#     author: jcz
# 
#==============================================================================
# 可自定义以下变量
# Name=test
# Inc=
# Lib=
#==============================================================================

Name=test
Include=-I./

CC=g++
AR=ar cr
RM=rm -rf

CCFlag= -c -g -O3 -Wall -Wno-deprecated -Wno-reorder -DLINUX2 -std=c++11 -D_DEBUG

BaseDir=./
ObjDir=$(BaseDir)/Obj/

SubDir=$(patsubst %/, %, $(patsubst %:, % , $(shell ls -R $(BaseDir) | grep "/")))
SubDir:=$(addsuffix /, $(SubDir))

VPATH=$(SubDir)

Source=$(foreach subdir, ${SubDir}, $(wildcard ${subdir}*.cpp))
Src=$(notdir $(Source))
SrcObj=$(patsubst %.cpp,%.o,$(Src))
Obj=$(addprefix $(ObjDir), $(SrcObj))

#==============================================================================

.PHONY:clean

all:$(ObjDir) $(Name)

$(ObjDir):
	@if [ ! -d $(ObjDir) ]; then mkdir $(ObjDir); fi

$(Name): $(Obj)
	@if [[ $(Name) == *.a ]]; then $(AR) $(Name) $(Obj); else $(CC) -o $(Name) $(Obj) $(Lib); fi
	@echo -e '\e[33m===== Create $(Name) =====\e[m'

$(ObjDir)%.o: %.cpp
	@$(CC) $(CCFlag) $< -o $@ $(Inc) $(Include)
	@echo -e 'Compile [$(Name)] $< -> $@'

clean:
	@$(RM) $(Name) $(ObjDir)
	@echo clean ... $(Name)
