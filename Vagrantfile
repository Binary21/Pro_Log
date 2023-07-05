# provision script
#-----------------------------------------------------------------------
$bootstrap = <<BOOTSTRAP
export DEBIAN_FRONTEND=noninteractive
apt-get update && apt-get upgrade

# label as reference environment
echo "export ECE3574_REFERENCE_ENV=Y" >> /etc/environment

# install base development tools
apt-get -y install build-essential
apt-get -y install cmake valgrind lcov
apt-get -y install python3-pip
pip3 install gcovr
pip3 install pexpect

# install a lightweight graphical environment
# for milestones 11 and 12 

#apt-get -y install xorg sddm openbox
#echo "[Autologin]" >> /etc/sddm.conf
#echo "User=vagrant" >> /etc/sddm.conf
#echo "Session=openbox.desktop" >> /etc/sddm.conf
#apt-get -y install qt5-default

BOOTSTRAP
#-----------------------------------------------------------------------

Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/bionic64"

  config.vm.boot_timeout = 600
  
  config.vm.provider "virtualbox" do |vb|
     vb.customize ["modifyvm", :id, "--uart1", "0x3F8", "4"]
     vb.customize ["modifyvm", :id, "--uartmode1", "file", File::NULL]
     #vb.gui = true
     #vb.customize ['setextradata', :id, 'GUI/ScaleFactor', '2.0']
  end

  # setup the VM
  config.vm.provision "shell", inline: $bootstrap  
end

