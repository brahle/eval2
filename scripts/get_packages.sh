# Install required packages 
sudo apt-get install gcc g++ python2.7 git-all python-setuptools 
sudo apt-get install mysql-server mysql-client postgresql

# Get and install Django
wget "http://www.djangoproject.com/download/1.2.4/tarball/"
mv "index.html" "Django-1.2.4.tar.gz"
tar xzvf "Django-1.2.4.tar.gz"
cd "Django-1.2.4"
sudo python setup.py install
cd ..
rm -rf "Django-1.2.4.tar.gz"
sudo rm -rf "Django-1.2.4"

