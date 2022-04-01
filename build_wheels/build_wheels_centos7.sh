function install_dependencies() {
local PARALLEL_NUM=$1
# need working pip3 command
yum install -y python3-pip
# boost only is not enough. need boost-devel which contains headers and libraries needed
# also for armadillo, you need armadillo-devel where -devel means develpment env including headers and libs
yum install -y armadillo-devel 
yum install -y atlas-devel blas lapack  # needed for python3.10 to build scipy
yum install -y yaml-cpp-devel openssl-devel libssh2-devel

# the boost-devel in centos 7 is too old
yum install -y wget xz
wget -c https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.gz
tar -xvf boost_1_78_0.tar.gz
cd boost_1_78_0
./bootstrap.sh
./b2 install -j ${PARALLEL_NUM}
}

function start_build() {
local PARALLEL_NUM=$1
for py in cp36-cp36m cp37-cp37m cp38-cp38 cp39-cp39 cp310-cp310
do
/opt/python/${py}/bin/pip install --user scikit-build cython pybind11[global]
done

cd /root/atomsciflow/
for py in cp36-cp36m cp37-cp37m cp38-cp38 cp39-cp39 cp310-cp310
do
# old build might destroy current build, so remove it
rm -rf _skbuild
/opt/python/${py}/bin/python3 setup.py build bdist_wheel -j ${PARALLEL_NUM}
done

for whl in /root/atomsciflow/dist/atomsciflow-*-linux_*.whl
do
auditwheel repair ${whl} --plat $PLAT -w /root/atomsciflow/dist
done
}

while getopts ":p:" arg; do
case ${arg} in
p)
PARALLEL_NUM="${OPTARG}"
;;
esac
done

echo ${PARALLEL_NUM}
install_dependencies ${PARALLEL_NUM}
start_build ${PARALLEL_NUM}
