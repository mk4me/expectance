mkdir ..\_doc
doxygen Generator.doxy  S
rmdir /S /Q "../_doc/html"
rmdir /S /Q "../_doc/rtf"
cd ..\doc
call gen_doc.cmd
cd ..
copy _doc\generator.chm doc

rmdir /S /Q _doc