#include "configParser.hpp"
#include <dirent.h>
#include <stdio.h>

static std::string join(char *path, char *file_name)
{
	std::string p(path);
	std::string f(file_name);
	return p + f;
}

int main(int argc, char **argv) {
	(void)argv;
    DIR *dir;
    struct dirent *dp;
    char dirpath[] = "./invalid_test/";
    int	i;

    i = 0;
    dir = NULL;
    if (argc > 1)
    {
    	i = 1;
//	while (argv[i] != NULL)
//	{
//	    tester(ft_strjoin(dirpath, argv[i]));
//	    i++;
//	}
    }
    else {
        dir = opendir(dirpath);
    	if (dir == NULL) { return 1; }
    	dp = readdir(dir);
    	while (dp != NULL) {
    	    printf("%s\n", dp->d_name);
	    if (dp->d_name[0] != '.')
    	    dp = readdir(dir);
			std::string txt = readConfFile(join(dirpath, dp->d_name));
			configParser confParser(txt);
			confParser.parseConf();
    	}
    }
}
