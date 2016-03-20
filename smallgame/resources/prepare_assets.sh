
#Copy assets to new folder
echo \*\* Copying assets folder to temporary folder...
cp -r ../assets/ assets_temp/

#Remove all .png extensions
echo \*\* Removing .png extensions...
find ./assets_temp/ -type f -name '*.png' | while read f;
	do mv "$f" "${f%.png}";
done

#Remove all .ogg extensions
echo \*\* Removing .ogg extensions...
find ./assets_temp/ -type f -name '*.ogg' | while read f;
        do mv "$f" "${f%.ogg}";
done

#Compile all .gs scripts
echo \*\* Compiling Gongly Scripts...
mkdir ./assets_temp/scripts
cd ../scripts
find ./ -type f -name '*.gs' | while read f;
	do echo \*\*\* Compiling "$f"... && ggsc "$f" -o ../resources/assets_temp/scripts/"${f%.gs}";
done
cd ../resources

#Create compressed archive
echo \*\* Creating compressed folder...
7z a data.zip ./assets_temp/* > /dev/null
echo \*\* Moving zipped data dir to folder...
mv data.zip ../data.dat

#Remove temporary dir
echo \*\* Removing temporary folder...
rm -rf assets_temp/
