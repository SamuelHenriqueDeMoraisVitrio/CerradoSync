


rm -f CerradoSync.h
curl -L https://github.com/OUIsolutions/OuiPacker/releases/download/0.005/OuiPacker.out -o packer
chmod +x packer
echo "./packer --folder_mode build"
./packer --folder_mode build -h


