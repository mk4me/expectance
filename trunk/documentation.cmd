mkdir _doc
doxygen Generator.doxy  S
rmdir /S /Q "_doc/html"
rmdir /S /Q "_doc/rtf"
cd doc
gen_doc.cmd
cd ..
