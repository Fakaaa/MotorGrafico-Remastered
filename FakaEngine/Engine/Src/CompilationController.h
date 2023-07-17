#ifndef COMPILATIONCONTROLLER_H
#define COMPILATIONCONTROLLER_H

static enum TypeCompilation
{
	Debug,
	Exe,
};

static TypeCompilation typeCompilation = TypeCompilation::Exe;
#endif