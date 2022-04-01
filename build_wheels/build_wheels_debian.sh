function modify_apt_sources_list() {
cp /etc/apt/sources.list /etc/apt/sources.list.bak
cat >/etc/apt/sources.list<<EOF
deb http://mirrors.aliyun.com/debian stretch main contrib non-free
deb http://mirrors.aliyun.com/debian stretch-proposed-updates main contrib non-free
deb http://mirrors.aliyun.com/debian stretch-updates main contrib non-free
deb-src http://mirrors.aliyun.com/debian stretch main contrib non-free
deb-src http://mirrors.aliyun.com/debian stretch-proposed-updates main contrib non-free
deb-src http://mirrors.aliyun.com/debian stretch-updates main contrib non-free
deb http://mirrors.aliyun.com/debian-security/ stretch/updates main non-free contrib
deb-src http://mirrors.aliyun.com/debian-security/ stretch/updates main non-free contrib
EOF
}

function install_dependencies() {
apt update
apt install -y libarmadillo-dev 
apt install -y libboost-all-dev # including atomic filesystem  program-options etc. but may failed to download somtimes
apt install -y --fix-missing
# so we install them individually again
apt install -y libboost-program-options-dev libboost-filesystem-dev libboost-system-dev

apt install -y libatlas-dev libblas-dev liblapack-dev  # needed for python3.10 to build scipy
# need working pip3 command
apt install -y python3-pip
apt install -y libyaml-cpp-dev libssh2-1 libssh2-1-dev 
# in Debian, the dev and header files for OpenSSL are provided by libssl-dev package
apt install -y openssl libssl-dev
}

function start_build() {
local PARALLEL_NUM=$1
for py in cp36-cp36m cp37-cp37m cp38-cp38 cp39-cp39 cp310-cp310
do
/opt/python/${py}/bin/pip3 install --user scikit-build cython pybind11[global]
done

cd /root/atomsciflow/
for py in cp36-cp36m cp37-cp37m cp38-cp38 cp39-cp39 cp310-cp310
do
# old build might destroy current build, so remove it
rm -rf _skbuild
/opt/python/${py}/bin/python3 setup.py bdist_wheel -j ${PARALLEL_NUM}
done

for whl in /root/atomsciflow/dist/atomsciflow*-linux_*.whl
do
auditwheel repair ${whl} -w /root/atomsciflow/dist
done

}

while getopts ":p:m:" arg; do
case ${arg} in
p)
PARALLEL_NUM="${OPTARG}"
;;
m)
MODIFY_APT_SOURCES="${OPTARG}"
;;
esac
done

echo ${PARALLEL_NUM}
echo ${MODIFY_APT_SOURCES}

if [[ ${MODIFY_APT_SOURCES} == *true* ]];then
modify_apt_sources_list
fi

install_dependencies
start_build ${PARALLEL_NUM}
