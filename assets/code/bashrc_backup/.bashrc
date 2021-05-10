# ~/.bashrc: executed by bash(1) for non-login shells.
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

# If not running interactively, don't do anything
case $- in
    *i*) ;;
      *) return;;
esac

# don't put duplicate lines or lines starting with space in the history.
# See bash(1) for more options
HISTCONTROL=ignoreboth

# append to the history file, don't overwrite it
shopt -s histappend

# for setting history length see HISTSIZE and HISTFILESIZE in bash(1)
HISTSIZE=1000
HISTFILESIZE=2000

# check the window size after each command and, if necessary,
# update the values of LINES and COLUMNS.
shopt -s checkwinsize

# If set, the pattern "**" used in a pathname expansion context will
# match all files and zero or more directories and subdirectories.
#shopt -s globstar

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
if [ -z "${debian_chroot:-}" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# set a fancy prompt (non-color, unless we know we "want" color)
case "$TERM" in
    xterm-color|*-256color) color_prompt=yes;;
esac

# uncomment for a colored prompt, if the terminal has the capability; turned
# off by default to not distract the user: the focus in a terminal window
# should be on the output of commands, not on the prompt
#force_color_prompt=yes

if [ -n "$force_color_prompt" ]; then
    if [ -x /usr/bin/tput ] && tput setaf 1 >&/dev/null; then
	# We have color support; assume it's compliant with Ecma-48
	# (ISO/IEC-6429). (Lack of such support is extremely rare, and such
	# a case would tend to support setf rather than setaf.)
	color_prompt=yes
    else
	color_prompt=
    fi
fi

if [ "$color_prompt" = yes ]; then
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;34m\]\w\[\033[00m\]\$ '
else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
unset color_prompt force_color_prompt

# If this is an xterm set the title to user@host:dir
case "$TERM" in
xterm*|rxvt*)
    PS1="\[\e]0;${debian_chroot:+($debian_chroot)}\u@\h: \w\a\]$PS1"
    ;;
*)
    ;;
esac

# enable color support of ls and also add handy aliases
if [ -x /usr/bin/dircolors ]; then
    test -r ~/.dircolors && eval "$(dircolors -b ~/.dircolors)" || eval "$(dircolors -b)"
    alias ls='ls --color=auto'
    #alias dir='dir --color=auto'
    #alias vdir='vdir --color=auto'

    alias grep='grep --color=auto'
    alias fgrep='fgrep --color=auto'
    alias egrep='egrep --color=auto'
fi

# colored GCC warnings and errors
#export GCC_COLORS='error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01'

# some more ls aliases
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

# Add an "alert" alias for long running commands.  Use like so:
#   sleep 10; alert
alias alert='notify-send --urgency=low -i "$([ $? = 0 ] && echo terminal || echo error)" "$(history|tail -n1|sed -e '\''s/^\s*[0-9]\+\s*//;s/[;&|]\s*alert$//'\'')"'

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

if [ -f ~/.bash_aliases ]; then
    . ~/.bash_aliases
fi

# enable programmable completion features (you don't need to enable
# this, if it's already enabled in /etc/bash.bashrc and /etc/profile
# sources /etc/bash.bashrc).
if ! shopt -oq posix; then
  if [ -f /usr/share/bash-completion/bash_completion ]; then
    . /usr/share/bash-completion/bash_completion
  elif [ -f /etc/bash_completion ]; then
    . /etc/bash_completion
  fi
fi

# basic software directory
export SOFTWARE_HOME=/home/zanghu/ProgramFiles/software

# added by Anaconda3 installer
#export PATH="/home/zanghu/ProgramFiles/software/anaconda3/anaconda3-5.1.0/bin:$PATH"

# 001.CUDA
export CUDA_HOME=/usr/local/cuda
export PATH=$CUDA_HOME/bin:$PATH
export C_INCLUDE_PATH=$CUDA_HOME/include:$C_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=$CUDA_HOME/include:$CPLUS_INCLUDE_PATH
export LD_LIBRARY_PATH=$CUDA_HOME/lib64:$LD_LIBRARY_PATH
export LIBRARY_PATH=$CUDA_HOME/lib64:$LIBRARY_PATH

# 002.LIBCUPTI
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda/extras/CUPTI/lib64
export LIBRARY_PATH=$LIBRARY_PATH:/usr/local/cuda/extras/CUPTI/lib64

# 003.bazel
export PATH=$PATH:/home/zanghu/ProgramFiles/software/bazel/bazel-0.13.0/bin

# 004.clang
#export CLANG_HOME=$SOFTWARE_HOME/clang/clang+llvm-6.0.0-x86_64-linux-gnu-ubuntu-16.04
#export PATH=$PATH:$CLANG_HOME/bin
#export C_INCLUDE_PATH=$C_INCLUDE_PATH:$CLANG_HOME/include
#export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$CLANG_HOME/include
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$CLANG_HOME/lib
#export LIBRARY_PATH=$LIBRARY_PATH:$CLANG_HOME/lib
#export MANPATH=$MANPATH:$CLANG_HOME/share/man

# 005.double-conversion
#export DOUBLE_CONVERSION_HOME=$SOFTWARE_HOME/double-conversion
#export C_INCLUDE_PATH=$C_INCLUDE_PATH:$DOUBLE_CONVERSION_HOME/include
#export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$DOUBLE_CONVERSION_HOME/include
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DOUBLE_CONVERSION_HOME
#export LIBRARY_PATH=$LIBRARY_PATH:$DOUBLE_CONVERSION_HOME

# 006.gsutil
export PATH=${PATH}:$SOFTWARE_HOME/gsutil

# 007.boost
export BOOST_HOME=$SOFTWARE_HOME/boost/boost_1_67_0
export C_INCLUDE_PATH=$C_INCLUDE_PATH:$BOOST_HOME/include
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$BOOST_HOME/include
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$BOOST_HOME/lib
export LIBRARY_PATH=$LIBRARY_PATH:$BOOST_HOME/lib
export BOOST_INCLUDE_DIR=$BOOST_HOME/include

# 008.zlib
export ZLIB_HOME=$SOFTWARE_HOME/zlib/zlib-1.2.11
export C_INCLUDE_PATH=$ZLIB_HOME/include:$C_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=$ZLIB_HOME/include:$CPLUS_INCLUDE_PATH
export LD_LIBRARY_PATH=$ZLIB_HOME/lib:$LD_LIBRARY_PATH
export LIBRARY_PATH=$ZLIB_HOME/lib:$LIBRARY_PATH
export MANPATH=$ZLIB_HOME/share/man:$MANPATH
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$ZLIB_HOME/lib/pkgconfig

# 009.openssl
export OPENSSL_HOME=$SOFTWARE_HOME/openssl/openssl-1.1.0h
export PATH=$OPENSSL_HOME/bin:$PATH
export C_INCLUDE_PATH=$OPENSSL_HOME/include:$C_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=$OPENSSL_HOME/include:$CPLUS_INCLUDE_PATH
export LD_LIBRARY_PATH=$OPENSSL_HOME/lib:$LD_LIBRARY_PATH
export LIBRARY_PATH=$OPENSSL_HOME/lib:$LIBRARY_PATH
export MANPATH=$OPENSSL_HOME/share/man:$MANPATH
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$OPENSSL_HOME/lib/pkgconfig

# 010.curl
export CURL_HOME=$SOFTWARE_HOME/curl/curl-7.59.0
export PATH=$CURL_HOME/bin:$PATH
export C_INCLUDE_PATH=$CURL_HOME/include:$C_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=$CURL_HOME/include:$CPLUS_INCLUDE_PATH
export LD_LIBRARY_PATH=$CURL_HOME/lib:$LD_LIBRARY_PATH
export LIBRARY_PATH=$CURL_HOME/lib:$LIBRARY_PATH
export MANPATH=$CURL_HOME/share/man:$MANPATH
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$CURL_HOME/lib/pkgconfig

# 011.go
#export GO_HOME=$SOFTWARE_HOME/go/go-1.14.6
export GO_HOME=$SOFTWARE_HOME/go/go1.16
export PATH=$GO_HOME/bin:$PATH

# 012.matlab
export MATLAB_HOME=/home/zanghu/ProgramFiles/software/matlab/matlab2018a
export PATH=$PATH:$MATLAB_HOME/bin
alias matlab="matlab -nodesktop -nosplash"
#alias matlab="matlab -nodisplay"

# 013.wget
export WGET_HOME=$SOFTWARE_HOME/wget/wget-1.19
export PATH=$WGET_HOME/bin:$PATH
export MANPATH=$WGET_HOME/share/man:$MANPATH

# 014.anaconda3
#export ANACONDA3_HOME=$SOFTWARE_HOME/anaconda3/anaconda3-5.3.0
#export C_INCLUDE_PATH=$ANACONDA3_HOME/include:$C_INCLUDE_PATH
#export CPLUS_INCLUDE_PATH=$ANACONDA3_HOME/include:$CPLUS_INCLUDE_PATH
#export LD_LIBRARY_PATH=$ANACONDA3_HOME/lib:$LD_LIBRARY_PATH
#export LIBRARY_PATH=$ANACONDA3_HOME/lib:$LIBRARY_PATH

# 015.valgrind
export VALGRIND_HOME=$SOFTWARE_HOME/valgrind/valgrind-3.15.0
export PATH=$VALGRIND_HOME/bin:$PATH
export MANPATH=$MANPATH:$VALGRIND_HOME/share/man
export PKG_CONFIG_PATH=$VALGRIND_HOME/lib/pkgconfig:$PKG_CONFIG_PATH

# 016.lcov
export LCOV_HOME=$SOFTWARE_HOME/lcov/lcov-1.13
export PATH=$PATH:$LCOV_HOME/bin
export MANPATH=$LCOV_HOME/share/man:$MANPATH

# 017.cmake
export CMAKE_HOME=$SOFTWARE_HOME/cmake/cmake-3.15.0
export CMAKE_ROOT=$CMAKE_HOME
export PATH=$CMAKE_HOME/bin:$PATH
#export MANPATH=$CMAKE_HOME/share/man:$MANPATH

# 018.opencv
export OPENCV_HOME=$SOFTWARE_HOME/opencv/opencv-3.4.3
export PATH=$OPENCV_HOME/bin:$PATH
export C_INCLUDE_PATH=$OPENCV_HOME/include:$C_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=$OPENCV_HOME/include:$CPLUS_INCLUDE_PATH
export LD_LIBRARY_PATH=$OPENCV_HOME/lib:$LD_LIBRARY_PATH
export LIBRARY_PATH=$OPENCV_HOME/lib:$LIBRARY_PATH
export MANPATH=$OPENCV_HOME/share/man:$MANPATH
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$OPENCV_HOME/lib/pkgconfig

# 019.protobuf
export PROTOC_HOME=$SOFTWARE_HOME/protobuf/protobuf-3.5.1.1
export PATH=$PROTOC_HOME/bin:$PATH
export C_INCLUDE_PATH=$PROTOC_HOME/include:$C_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=$PROTOC_HOME/include:$CPLUS_INCLUDE_PATH
export LD_LIBRARY_PATH=$PROTOC_HOME/lib:$LD_LIBRARY_PATH
export LIBRARY_PATH=$PROTOC_HOME/lib:$LIBRARY_PATH
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$PROTOC_HOME/lib/pkgconfig

# 020.readline
#export READLINE_HOME=$SOFTWARE_HOME/readline/readline-6.3
#export PATH=$READLINE_HOME/bin:$PATH
#export C_INCLUDE_PATH=$READLINE_HOME/include:$C_INCLUDE_PATH
#export CPLUS_INCLUDE_PATH=$READLINE_HOME/include:$CPLUS_INCLUDE_PATH
#export LD_LIBRARY_PATH=$READLINE_HOME/lib:$LD_LIBRARY_PATH
#export LIBRARY_PATH=$READLINE_HOME/lib:$LIBRARY_PATH
#export MANPATH=$READLINE_HOME/share/man:$MANPATH

# 021.node
export NODEJS_HOME=$SOFTWARE_HOME/node/node-v12.13.1
export PATH=$NODEJS_HOME/bin:$PATH
export MANPATH=$NODEJS_HOME/share/man:$MANPATH

# 022.redis
export REDIS_HOME=$SOFTWARE_HOME/redis/redis-5.0.7
export PATH=$REDIS_HOME/bin:$PATH


# added by Anaconda2 installer
#export PATH="/home/zanghu/ProgramFiles/softwareanaconda2/anaconda2-5.1.0/bin:$PATH"

# added by Anaconda3 installer
#export PATH="/home/zanghu/ProgramFiles/software/anaconda3/anaconda3-5.1.0/bin:$PATH"

# The next line updates PATH for the Google Cloud SDK.
#if [ -f '/home/zanghu/soft_setup/google-cloud-sdk/path.bash.inc' ]; then source '/home/zanghu/soft_setup/google-cloud-sdk/path.bash.inc'; fi

# The next line enables shell command completion for gcloud.
#if [ -f '/home/zanghu/soft_setup/google-cloud-sdk/completion.bash.inc' ]; then source '/home/zanghu/soft_setup/google-cloud-sdk/completion.bash.inc'; fi

# proxy
#export http_proxy=http://192.168.3.2:1080
#export https_proxy=http://192.168.3.2:1080
#export no_proxy=192.168.*.*,*.local,localhost,127.*.*.*
#http_proxy=10.3.8.33:1080  
#https_proxy=$http_proxy  

#export http_proxy=http://192.168.3.20:1080
#export https_proxy=http://192.168.3.20:1080
#export no_proxy=192.168.*.*,*.local,localhost,127.*.*.*

# data base
export ML_DB_HDD=/home/zanghu/data_base
export ML_DB_SSD=/mnt/ssd_01/data_base

# alias
alias ipynb="jupyter notebook --certfile=/home/zanghu/ProgramFiles/ipynb_ssl/mycert.pem --keyfile /home/zanghu/ProgramFiles/ipynb_ssl/mykey.key"

export PYTHONPATH=$PYTHONPATH:/mnt/ssd_01/jane-street-market-prediction:/home/zanghu/ProgramFiles/software/python_third:/home/zanghu/git_box/models:/home/zanghu/git_box/models/research/slim
#:/home/zanghu/git_box/models:

# torch.utils.model_zoo下的预训练模型的自定义保存路径, 如果不设置默认下载到~/.torch目录下
export TORCH_MODEL_ZOO=/home/zanghu/model_zoo/torchvision_models

# 避免xshell远程plt保存图片出错
export DISPLAY=':0.0'
export QT_QPA_PLATFORM='offscreen'

# SurPRISE数据集目录
export SURPRISE_DATA_FOLDER=/home/zanghu/data_base

# EDA工具
alias eda_tags="python /home/zanghu/data_base/eda_tools/eda_preprocess.py"

# >>> conda initialize >>>
# !! Contents within this block are managed by 'conda init' !!
__conda_setup="$('/home/zanghu/ProgramFiles/software/anaconda3/2019.03/bin/conda' 'shell.bash' 'hook' 2> /dev/null)"
if [ $? -eq 0 ]; then
    eval "$__conda_setup"
else
    if [ -f "/home/zanghu/ProgramFiles/software/anaconda3/2019.03/etc/profile.d/conda.sh" ]; then
        . "/home/zanghu/ProgramFiles/software/anaconda3/2019.03/etc/profile.d/conda.sh"
    else
        export PATH="/home/zanghu/ProgramFiles/software/anaconda3/2019.03/bin:$PATH"
    fi
fi
unset __conda_setup
# <<< conda initialize <<<

