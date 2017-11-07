#include "PngDecoder.h"

#include <Windows.h>
#include "png.h"

/* Utility to save typing/errors, the argument must be a name */
#define MEMZERO(var) ((void)memset(&var, 0, sizeof var))
#define FCLOSE(file) fclose(file)

class WOSBitmap
{
public:
	WOSBitmap()
	{
		m_iWidth = 0;
		m_iHeight = 0;
		m_pData = nullptr;
	}
	~WOSBitmap()
	{
		if (m_pData)
			free(m_pData);
		m_pData = nullptr;
	}
public:
	int m_iWidth;
	int m_iHeight;
	char* m_pData;
};

static int verbose = 0;
static int strict = 0;
static int relaxed = 0;
static int error_count = 0; /* count calls to png_error */
static int warning_count = 0; /* count calls to png_warning */

typedef struct
{
	PNG_CONST char *file_name;
}  pngtest_error_parameters;

static void PNGCBAPI pngtest_warning(png_structp png_ptr, png_const_charp message)
{
	PNG_CONST char *name = "UNKNOWN (ERROR!)";
	pngtest_error_parameters *test =
		(pngtest_error_parameters*)png_get_error_ptr(png_ptr);

	++warning_count;

	if (test != NULL && test->file_name != NULL)
		name = test->file_name;

	//fprintf(STDERR, "\n%s: libpng warning: %s\n", name, message);
}
static void PNGCBAPI pngtest_error(png_structp png_ptr, png_const_charp message)
{
	++error_count;

	pngtest_warning(png_ptr, message);
	/* We can return because png_error calls the default handler, which is
	* actually OK in this case.
	*/
}

static struct user_chunk_data
{
	png_const_infop info_ptr;
	png_uint_32     vpAg_width, vpAg_height;
	png_byte        vpAg_units;
	png_byte        sTER_mode;
	int             location[2];
}
user_chunk_data;

/* Used for location and order; zero means nothing. */
#define have_sTER   0x01
#define have_vpAg   0x02
#define before_PLTE 0x10
#define before_IDAT 0x20
#define after_IDAT  0x40

static void
	init_callback_info(png_const_infop info_ptr)
{
	MEMZERO(user_chunk_data);
	user_chunk_data.info_ptr = info_ptr;
}

/* Example of using row callbacks to make a simple progress meter */
static int status_pass = 1;
static int status_dots_requested = 0;
static int status_dots = 1;

static void PNGCBAPI
	read_row_callback(png_structp png_ptr, png_uint_32 row_number, int pass)
{
	if (png_ptr == NULL || row_number > PNG_UINT_31_MAX)
		return;

	if (status_pass != pass)
	{
		fprintf(stdout, "\n Pass %d: ", pass);
		status_pass = pass;
		status_dots = 31;
	}

	status_dots--;

	if (status_dots == 0)
	{
		fprintf(stdout, "\n         ");
		status_dots=30;
	}

	fprintf(stdout, "r");
}

#ifdef PNG_READ_USER_TRANSFORM_SUPPORTED
/* Example of using a user transform callback (doesn't do anything at present).
*/
static void PNGCBAPI
	read_user_callback(png_structp png_ptr, png_row_infop row_info, png_bytep data)
{
}
#endif

static int PNGCBAPI read_user_chunk_callback(png_struct *png_ptr, png_unknown_chunkp chunk)
{
	return (1);
}

int readPng(const char * inname, int& bwidth, int& bheight, char** bcbData)
{
	static png_FILE_p fpin;
	//static png_FILE_p fpout;  /* "static" prevents setjmp corruption */
	pngtest_error_parameters error_parameters;
	png_structp read_ptr;
	png_infop read_info_ptr, end_info_ptr;

	png_bytep row_buf = NULL;
	png_uint_32 y;
	png_uint_32 width, height;
	volatile int num_passes;
	int pass;
	int bit_depth, color_type;

	if ((fpin = fopen(inname, "rb")) == NULL)
	{
		//fprintf(STDERR, "Could not find input file %s\n", inname);
		return (1);
	}

	read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_set_error_fn(read_ptr, &error_parameters, pngtest_error, pngtest_warning);

	read_info_ptr = png_create_info_struct(read_ptr);
	end_info_ptr = png_create_info_struct(read_ptr);

#ifdef PNG_READ_USER_CHUNKS_SUPPORTED
	init_callback_info(read_info_ptr);
	png_set_read_user_chunk_fn(read_ptr, &user_chunk_data, read_user_chunk_callback);
#endif

#ifdef PNG_SETJMP_SUPPORTED
	//pngtest_debug("Setting jmpbuf for read struct");
	if (setjmp(png_jmpbuf(read_ptr)))
	{
		//fprintf(STDERR, "%s -> %s: libpng read error\n", inname, outname);
		png_free(read_ptr, row_buf);
		row_buf = NULL;
		if (verbose != 0)
		{
			//fprintf(STDERR, "   destroy read structs\n");
		}
		png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);

		FCLOSE(fpin);
		return (1);
	}
#endif

#ifdef PNG_BENIGN_ERRORS_SUPPORTED
	if (strict != 0)
	{
		/* Treat png_benign_error() as errors on read */
		png_set_benign_errors(read_ptr, 0);

		/* if strict is not set, then app warnings and errors are treated as
		* warnings in release builds, but not in unstable builds; this can be
		* changed with '--relaxed'.
		*/
	}

	else if (relaxed != 0)
	{
		/* Allow application (pngtest) errors and warnings to pass */
		png_set_benign_errors(read_ptr, 1);

		/* Turn off CRC checking while reading */
		png_set_crc_action(read_ptr, PNG_CRC_QUIET_USE, PNG_CRC_QUIET_USE);

#ifdef PNG_IGNORE_ADLER32
		/* Turn off ADLER32 checking while reading */
		png_set_option(read_ptr, PNG_IGNORE_ADLER32, PNG_OPTION_ON);
#endif

	}
#endif /* BENIGN_ERRORS */

	//pngtest_debug("Initializing input and output streams");
#ifdef PNG_STDIO_SUPPORTED
	png_init_io(read_ptr, fpin);
#else
	png_set_read_fn(read_ptr, (png_voidp)fpin, pngtest_read_data);
#endif

	if (status_dots_requested == 1)
	{
		png_set_read_status_fn(read_ptr, read_row_callback);
	}
	else
	{
		png_set_read_status_fn(read_ptr, NULL);
	}

#ifdef PNG_READ_USER_TRANSFORM_SUPPORTED
	png_set_read_user_transform_fn(read_ptr, read_user_callback);
#endif

	//pngtest_debug("Reading info struct");
	png_read_info(read_ptr, read_info_ptr);

#ifdef PNG_READ_USER_CHUNKS_SUPPORTED
	/* This is a bit of a hack; there is no obvious way in the callback function
	* to determine that the chunks before the first IDAT have been read, so
	* remove the info_ptr (which is only used to determine position relative to
	* PLTE) here to indicate that we are after the IDAT.
	*/
	user_chunk_data.info_ptr = NULL;
#endif

	//pngtest_debug("Transferring info struct");
	{
		int interlace_type, compression_type, filter_type;

		if (png_get_IHDR(read_ptr, read_info_ptr, &width, &height, &bit_depth,
			&color_type, &interlace_type, &compression_type, &filter_type) != 0)
		{
			/* num_passes may not be available below if interlace support is not
			* provided by libpng for both read and write.
			*/
			switch (interlace_type)
			{
			case PNG_INTERLACE_NONE:
				num_passes = 1;
				break;

			case PNG_INTERLACE_ADAM7:
				num_passes = 7;
				break;

			default:
				png_error(read_ptr, "invalid interlace type");
				/*NOT REACHED*/
			}
		}

		else
			png_error(read_ptr, "png_get_IHDR failed");
	}

#ifdef PNG_FIXED_POINT_SUPPORTED
#ifdef PNG_cHRM_SUPPORTED
	{
		png_fixed_point white_x, white_y, red_x, red_y, green_x, green_y, blue_x,
			blue_y;

		if (png_get_cHRM_fixed(read_ptr, read_info_ptr, &white_x, &white_y,
			&red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y) != 0)
		{

		}
	}
#endif
#ifdef PNG_gAMA_SUPPORTED
	{
		png_fixed_point gamma;

		if (png_get_gAMA_fixed(read_ptr, read_info_ptr, &gamma) != 0)
		{
		}
	}
#endif
#else /* Use floating point versions */
#endif /* Fixed point */
#ifdef PNG_iCCP_SUPPORTED
	{
		png_charp name;
		png_bytep profile;
		png_uint_32 proflen;
		int compression_type;

		if (png_get_iCCP(read_ptr, read_info_ptr, &name, &compression_type,
			&profile, &proflen) != 0)
		{

		}
	}
#endif
#ifdef PNG_sRGB_SUPPORTED
	{
		int intent;

		if (png_get_sRGB(read_ptr, read_info_ptr, &intent) != 0)
		{
		}
	}
#endif
	{
		png_colorp palette;
		int num_palette;

		if (png_get_PLTE(read_ptr, read_info_ptr, &palette, &num_palette) != 0)
		{
		}
	}
#ifdef PNG_bKGD_SUPPORTED
	{
		png_color_16p background;

		if (png_get_bKGD(read_ptr, read_info_ptr, &background) != 0)
		{

		}
	}
#endif
#ifdef PNG_READ_eXIf_SUPPORTED
	{
		png_bytep exif=NULL;
		png_uint_32 exif_length;

		if (png_get_eXIf_1(read_ptr, read_info_ptr, &exif_length, &exif) != 0)
		{
			if (exif_length > 1)
			{
			}
		}
	}
#endif
#ifdef PNG_hIST_SUPPORTED
	{
		png_uint_16p hist;

		if (png_get_hIST(read_ptr, read_info_ptr, &hist) != 0)
			;
	}
#endif
#ifdef PNG_oFFs_SUPPORTED
	{
		png_int_32 offset_x, offset_y;
		int unit_type;

		if (png_get_oFFs(read_ptr, read_info_ptr, &offset_x, &offset_y,
			&unit_type) != 0)
		{

		}
	}
#endif
#ifdef PNG_pCAL_SUPPORTED
	{
		png_charp purpose, units;
		png_charpp params;
		png_int_32 X0, X1;
		int type, nparams;

		if (png_get_pCAL(read_ptr, read_info_ptr, &purpose, &X0, &X1, &type,
			&nparams, &units, &params) != 0)
		{

		}
	}
#endif
#ifdef PNG_pHYs_SUPPORTED
	{
		png_uint_32 res_x, res_y;
		int unit_type;

		if (png_get_pHYs(read_ptr, read_info_ptr, &res_x, &res_y,
			&unit_type) != 0)
			;
	}
#endif
#ifdef PNG_sBIT_SUPPORTED
	{
		png_color_8p sig_bit;

		if (png_get_sBIT(read_ptr, read_info_ptr, &sig_bit) != 0)
			;
	}
#endif
#ifdef PNG_sCAL_SUPPORTED
#if defined(PNG_FLOATING_POINT_SUPPORTED) && \
	defined(PNG_FLOATING_ARITHMETIC_SUPPORTED)
	{
		int unit;
		double scal_width, scal_height;

		if (png_get_sCAL(read_ptr, read_info_ptr, &unit, &scal_width,
			&scal_height) != 0)
		{

		}
	}
#else
#ifdef PNG_FIXED_POINT_SUPPORTED
	{
		int unit;
		png_charp scal_width, scal_height;

		if (png_get_sCAL_s(read_ptr, read_info_ptr, &unit, &scal_width,
			&scal_height) != 0)
		{

		}
	}
#endif
#endif
#endif

#ifdef PNG_sPLT_SUPPORTED
	{
		png_sPLT_tp entries;

		int num_entries = (int) png_get_sPLT(read_ptr, read_info_ptr, &entries);
		if (num_entries)
		{

		}
	}
#endif

#ifdef PNG_TEXT_SUPPORTED
	{
		png_textp text_ptr;
		int num_text;

		if (png_get_text(read_ptr, read_info_ptr, &text_ptr, &num_text) > 0)
		{
			//pngtest_debug1("Handling %d iTXt/tEXt/zTXt chunks", num_text);

			//pngtest_check_text_support(read_ptr, text_ptr, num_text);

			if (verbose != 0)
			{
				int i;

				//fprintf(STDERR,"\n");
				for (i=0; i<num_text; i++)
				{

				}
			}

		}
	}
#endif
#ifdef PNG_tIME_SUPPORTED
	{
		png_timep mod_time;

		if (png_get_tIME(read_ptr, read_info_ptr, &mod_time) != 0)
		{
		}
	}
#endif
#ifdef PNG_tRNS_SUPPORTED
	{
		png_bytep trans_alpha;
		int num_trans;
		png_color_16p trans_color;

		if (png_get_tRNS(read_ptr, read_info_ptr, &trans_alpha, &num_trans,
			&trans_color) != 0)
		{
			int sample_max = (1 << bit_depth);
			/* libpng doesn't reject a tRNS chunk with out-of-range samples */
			if (!((color_type == PNG_COLOR_TYPE_GRAY &&
				(int)trans_color->gray > sample_max) ||
				(color_type == PNG_COLOR_TYPE_RGB &&
				((int)trans_color->red > sample_max ||
				(int)trans_color->green > sample_max ||
				(int)trans_color->blue > sample_max))))
			{
			}
		}
	}
#endif
#ifdef PNG_WRITE_UNKNOWN_CHUNKS_SUPPORTED
	{
		png_unknown_chunkp unknowns;
		int num_unknowns = png_get_unknown_chunks(read_ptr, read_info_ptr,
			&unknowns);

		if (num_unknowns != 0)
		{

		}
	}
#endif

#ifdef PNG_WRITE_SUPPORTED
	//pngtest_debug("Writing info struct");


#endif

#ifdef SINGLE_ROWBUF_ALLOC
	pngtest_debug("Allocating row buffer...");
	row_buf = (png_bytep)png_malloc(read_ptr,
		png_get_rowbytes(read_ptr, read_info_ptr));

	pngtest_debug1("\t0x%08lx", (unsigned long)row_buf);
#endif /* SINGLE_ROWBUF_ALLOC */
	//pngtest_debug("Writing row data");

#if defined(PNG_READ_INTERLACING_SUPPORTED) &&\
	defined(PNG_WRITE_INTERLACING_SUPPORTED)
	/* Both must be defined for libpng to be able to handle the interlace,
	* otherwise it gets handled below by simply reading and writing the passes
	* directly.
	*/
	if (png_set_interlace_handling(read_ptr) != num_passes)
		;

#else /* png_set_interlace_handling not called on either read or write */
#  define calc_pass_height
#endif /* not using libpng interlace handling */



#ifdef PNGTEST_TIMING
	t_stop = (float)clock();
	t_misc += (t_stop - t_start);
	t_start = t_stop;
#endif
	if (num_passes == 1)
	{
		bwidth = width;
		bheight = height;
		
		long file_size = width * height * 4;
		int block_size = 0;
		block_size = color_type == 6 ? 4 : 3;
		char *cbData = (char *)malloc(file_size);

		for (pass = 0; pass < 1; pass++)
		{
			//pngtest_debug1("Writing row data for pass %d", pass);
			for (y = 0; y < height; y++)
			{
#ifndef SINGLE_ROWBUF_ALLOC
				//pngtest_debug2("Allocating row buffer (pass %d, y = %u)...", pass, y);

				row_buf = (png_bytep)png_malloc(read_ptr,
					png_get_rowbytes(read_ptr, read_info_ptr));

				png_read_rows(read_ptr, (png_bytepp)&row_buf, NULL, 1);

				for (int x = 0; x < width; x ++)
				{
					cbData[y * (width * block_size) + x * block_size + 0] = row_buf[x * block_size + 2]; // B
					cbData[y * (width * block_size) + x * block_size + 1] = row_buf[x * block_size + 1]; // G
					cbData[y * (width * block_size) + x * block_size + 2] = row_buf[x * block_size + 0]; // R
					cbData[y * (width * block_size) + x * block_size + 3] = row_buf[x * block_size + 3]; // A
				}

				//pngtest_debug2("\t0x%08lx (%lu bytes)", (unsigned long)row_buf,
				//    (unsigned long)png_get_rowbytes(read_ptr, read_info_ptr));

#endif /* !SINGLE_ROWBUF_ALLOC */
				


#ifndef SINGLE_ROWBUF_ALLOC
				//pngtest_debug2("Freeing row buffer (pass %d, y = %u)", pass, y);
				png_free(read_ptr, row_buf);
				row_buf = NULL;
#endif /* !SINGLE_ROWBUF_ALLOC */
			}
		}
		*bcbData = cbData;
	}
	else 
	{
		for (pass = 0; pass < num_passes; pass++)
		{
#     ifdef calc_pass_height
			png_uint_32 pass_height;

			if (num_passes == 7) /* interlaced */
			{
				if (PNG_PASS_COLS(width, pass) > 0)
					pass_height = PNG_PASS_ROWS(height, pass);

				else
					pass_height = 0;
			}

			else /* not interlaced */
				pass_height = height;
#     else
#        define pass_height height
#     endif


			//pngtest_debug1("Writing row data for pass %d", pass);
			for (y = 0; y < pass_height; y++)
			{
#ifndef SINGLE_ROWBUF_ALLOC
				//pngtest_debug2("Allocating row buffer (pass %d, y = %u)...", pass, y);

				row_buf = (png_bytep)png_malloc(read_ptr,
					png_get_rowbytes(read_ptr, read_info_ptr));

				//pngtest_debug2("\t0x%08lx (%lu bytes)", (unsigned long)row_buf,
				//    (unsigned long)png_get_rowbytes(read_ptr, read_info_ptr));

#endif /* !SINGLE_ROWBUF_ALLOC */
				png_read_rows(read_ptr, (png_bytepp)&row_buf, NULL, 1);


#ifdef PNG_WRITE_SUPPORTED
#ifdef PNGTEST_TIMING
				t_stop = (float)clock();
				t_decode += (t_stop - t_start);
				t_start = t_stop;
#endif

#ifdef PNGTEST_TIMING
				t_stop = (float)clock();
				t_encode += (t_stop - t_start);
				t_start = t_stop;
#endif
#endif /* WRITE */

#ifndef SINGLE_ROWBUF_ALLOC
				//pngtest_debug2("Freeing row buffer (pass %d, y = %u)", pass, y);
				png_free(read_ptr, row_buf);
				row_buf = NULL;
#endif /* !SINGLE_ROWBUF_ALLOC */
			}
		}
	}


#ifdef PNG_STORE_UNKNOWN_CHUNKS_SUPPORTED
#  ifdef PNG_READ_UNKNOWN_CHUNKS_SUPPORTED
	png_free_data(read_ptr, read_info_ptr, PNG_FREE_UNKN, -1);
#  endif

#endif

	//pngtest_debug("Reading and writing end_info data");

	png_read_end(read_ptr, end_info_ptr);
#ifdef PNG_TEXT_SUPPORTED
	{
		png_textp text_ptr;
		int num_text;

		if (png_get_text(read_ptr, end_info_ptr, &text_ptr, &num_text) > 0)
		{

		}
	}
#endif
#ifdef PNG_READ_eXIf_SUPPORTED
	{
		png_bytep exif=NULL;
		png_uint_32 exif_length;

		if (png_get_eXIf_1(read_ptr, end_info_ptr, &exif_length, &exif) != 0)
		{

		}
	}
#endif
#ifdef PNG_tIME_SUPPORTED
	{
		png_timep mod_time;

		if (png_get_tIME(read_ptr, end_info_ptr, &mod_time) != 0)
		{

		}
	}
#endif
#ifdef PNG_WRITE_UNKNOWN_CHUNKS_SUPPORTED
	{
		png_unknown_chunkp unknowns;
		int num_unknowns = png_get_unknown_chunks(read_ptr, end_info_ptr,
			&unknowns);

		if (num_unknowns != 0)
		{

		}
	}
#endif


	//pngtest_debug("Destroying data structs");
#ifdef SINGLE_ROWBUF_ALLOC
	pngtest_debug("destroying row_buf for read_ptr");
	png_free(read_ptr, row_buf);
	row_buf = NULL;
#endif /* SINGLE_ROWBUF_ALLOC */
	//pngtest_debug("destroying read_ptr, read_info_ptr, end_info_ptr");
	png_destroy_read_struct(&read_ptr, &read_info_ptr, &end_info_ptr);

	//pngtest_debug("Destruction complete.");

	FCLOSE(fpin);

	/* Summarize any warnings or errors and in 'strict' mode fail the test.
	* Unsupported chunks can result in warnings, in that case ignore the strict
	* setting, otherwise fail the test on warnings as well as errors.
	*/
	if (error_count > 0)
	{
		/* We don't really expect to get here because of the setjmp handling
		* above, but this is safe.
		*/


		if (strict != 0)
			return (1);
	}


	else if (warning_count > 0)
	{

		if (strict != 0)
			return (1);
	}

	return 0;
}
