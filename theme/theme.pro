TEMPLATE = subdirs

isEmpty(PREFIX):PREFIX=/usr/local

theme.files = default
theme.path = $$PREFIX/share/antico/themes/

INSTALLS += theme
