#ifndef _TRACE_DBG_H_
#define _TRACE_DBG_H_

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

#define IPC_PARAM       0666
#define MAX_LINE        256

#ifdef USE_PIXEL_FORMAT_YUV_SEMIPLANAR_422
#define PIXEL_FORMAT    PIXEL_FORMAT_YUV_SEMIPLANAR_422
#define CHRM_SIZE_LEFT  1 /* YUV422(>> 1), YUV420(>> 2) */
#else
#define PIXEL_FORMAT    PIXEL_FORMAT_YUV_SEMIPLANAR_420
#define CHRM_SIZE_LEFT  2 /* YUV422(>> 1), YUV420(>> 2) */
#endif /* USE_PIXEL_FORMAT_YUV_SEMIPLANAR_422 */

#define PAUSE   do { getchar(); } while (0)
#define PERR    printf("%s>: failed at %s: LINE: %d, errno: %d, %s\n", __FILE__, __FUNCTION__, __LINE__, errno, strerror(errno));

/******************************************************************************
 * 修改提示字符颜色,对应颜色表
 * none         = "\033[0m"
 * black        = "\033[0;30m"
 * dark_gray    = "\033[1;30m"
 * blue         = "\033[0;34m"
 * light_blue   = "\033[1;34m"
 * green        = "\033[0;32m"
 * light_green -= "\033[1;32m"
 * cyan         = "\033[0;36m"
 * light_cyan   = "\033[1;36m"
 * red          = "\033[0;31m"
 * light_red    = "\033[1;31m"
 * purple       = "\033[0;35m"
 * light_purple = "\033[1;35m"
 * brown        = "\033[0;33m"
 * yellow       = "\033[1;33m"
 * light_gray   = "\033[0;37m"
 * white        = "\033[1;37m"
 *****************************************************************************/
#define COLOR_STR_NONE              "\033[0m"
#define COLOR_STR_BLACK             "\033[0;30m"
#define COLOR_STR_LIGHT_GRAY        "\033[0;37m"
#define COLOR_STR_DARK_GRAY         "\033[1;30m"
#define COLOR_STR_BLUE              "\033[0;34m"
#define COLOR_STR_LIGHT_BLUE        "\033[1;34m"
#define COLOR_STR_GREEN             "\033[0;32m"
#define COLOR_STR_LIGHT_GREEN       "\033[1;32m"
#define COLOR_STR_CYAN              "\033[0;36m"
#define COLOR_STR_LIGHT_CYAN        "\033[1;36m"
#define COLOR_STR_RED               "\033[0;31m"
#define COLOR_STR_LIGHT_RED         "\033[1;31m"
#define COLOR_STR_PURPLE            "\033[0;35m"
#define COLOR_STR_LIGHT_PURPLE      "\033[1;35m"
#define COLOR_STR_BROWN             "\033[0;33m"
#define COLOR_STR_YELLOW            "\033[1;33m"
#define COLOR_STR_WHITE             "\033[1;37m"

#define  ERR_DEBUG
#ifdef ERR_DEBUG
#define TRACE_ERR(str, args...)     printf(COLOR_STR_RED "%s>: FUNCTION: %s, LINE: %d, " COLOR_STR_NONE str, __FILE__, __FUNCTION__, __LINE__, ## args);
#define TRACE_DBG(str, args...)     printf(COLOR_STR_GREEN "%s>: FUNCTION: %s, LINE: %d, " COLOR_STR_NONE str, __FILE__, __FUNCTION__, __LINE__, ## args);
#define TRACE_PRI(str, args...)     printf(str, ## args);
#define TRACE(str, args...)			printf(str, ## args);
#else
#define TRACE_ERR(str, args...)
#define TRACE_DBG(str, args...)
#define TRACE_PRI(str, args...)
#define TRACE(str, args...)
#endif /* ERR_DEBUG */

//#define USER_DEBUG
#ifdef USER_DEBUG
#define TRACE_REL(str, args...)     printf("%s>: FUNCTION: %s, LINE: %d, "str, __FILE__, __FUNCTION__, __LINE__, ## args);
#else
#define TRACE_REL(str, args...)
#endif
#define TRACE_ALWAYS(str, args...)  printf(str, ## args);

#endif /* _TRACE_DBG_H_ */
