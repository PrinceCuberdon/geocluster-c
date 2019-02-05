/*
 * Geoclustering micro service 
 * (c) Prince Cuberdon 2018
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its 
 *    contributors may be used to endorse or promote products derived from 
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "point.h"
#include "log.h"

#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <memory.h>

/*
 * Check if the file exists, exit with a message elsewhere
 */
void file_ensure_exists(const char *filename)
{
    struct stat buffer;
    char exists = 0;

    exists = stat(filename, &buffer) == 0;
    if (!exists)
    {
        log_critical("Error: The file '%s' doesn't exist\n", filename);
        exit(1);
    }

}

/*
 * Load the file content or quit
 */
char *file_load(const char *filename)
{
    FILE *file = NULL;
    char *content = NULL;
    long length = 0;

    file_ensure_exists(filename);

    file = fopen(filename, "r");
    if (!file)
    {
        log_critical("Can't load file: %s", filename);
        exit(1);
    }
    // Get file size
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    if (length == -1)
    {
        log_critical("Unable to get the file size because : %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Read file
    fseek(file, 0, SEEK_SET);
    content = (char *) malloc(sizeof(char *) * length);
    if (!content)
    {
        log_critical("Unable to allocate memory because : %s", strerror(errno));
        exit(EXIT_FAILURE);
    }

    (void)fread(content, 1, (size_t)length, file);
    fclose(file);

    return content;
}
