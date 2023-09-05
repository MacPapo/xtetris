/**
 * @file   main.c
 * @author Jacopo Costantini Matteo Zambon Alvise Silvestri
 * @date   Fri Jan 28 00:05:07 2022
 *
 * @brief  The main file
 *
 * Where the magic happen
 *
 *
 */

#include "menu.h"
#include "commonConfing.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

#include "singlePlayer.h"
#include "multiPlayer.h"
#include "cpu.h"

#include "colors.h"
#include <ncurses.h>

/**
 * Qui si inizializza il loop dei decoder musicali
 *
 * @param pDevice
 * @param pOutput
 * @param pInput
 * @param frameCount
 */
void data_callback( ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount )
{
    ma_decoder* pDecoder = ( ma_decoder* )pDevice->pUserData;

    if (pDecoder == NULL)
        return;

    /* Reading PCM frames will loop based on what we specified when called ma_data_source_set_looping(). */
    ma_data_source_read_pcm_frames( pDecoder, pOutput, frameCount, NULL );

    ( void )pInput;
}

void paintExit()
{
    int starty, startx;
    WINDOW *w_byby;

    clear();
    raw();

    starty = ( ( LINES - BYBY_H ) / 2 );
    startx = ( ( COLS  - BYBY_W ) /  2 );

    w_byby = newwin( BYBY_H, BYBY_W, starty, startx );
    box( w_byby, BOX_VE, BOX_HO );
    wbkgd (w_byby, COLOR_PAIR( 2 ) );

    mvwprintw( w_byby, 0, 14, "+ BYBY SEE YOU NEXT TIME +" );
    mvwprintw( w_byby, 2, 3, "Hope you enjoied this simple game..." );
    mvwprintw( w_byby, 4, 3, "See you next time bro, have a nice day/night." );

    wattrset( w_byby, A_REVERSE );
    mvwprintw( w_byby, 6, 15, "PRESS ANY KEY TO EXIT..." );
    wattroff( w_byby, A_REVERSE );
    mvwprintw( w_byby, 8, 5, "+ By: @MacPapo @Blast291 @mastrodeimastri +" );

    refresh();
    wrefresh( w_byby );

    getch();
}

int main( int argc, char** argv )
{
    int mod = 0;

    ma_result        result;
    ma_decoder       decoder;
    ma_device_config deviceConfig;
    ma_device        device;

    if (argc < 2)
    {
        printf( "No input file.\n" );
        return ( -1 );
    }

    result = ma_decoder_init_file( argv[1], NULL, &decoder );

    if ( result != MA_SUCCESS )
        return -2;

    /*
    A decoder is a data source which means we just use ma_data_source_set_looping() to set the
    looping state. We will read data using ma_data_source_read_pcm_frames() in the data callback.
    */
    ma_data_source_set_looping( &decoder, MA_TRUE );

    deviceConfig                   = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate        = decoder.outputSampleRate;
    deviceConfig.dataCallback      = data_callback;
    deviceConfig.pUserData         = &decoder;

    if ( ma_device_init( NULL, &deviceConfig, &device ) != MA_SUCCESS )
    {
        printf( "Failed to open playback device.\n" );
        ma_decoder_uninit( &decoder );
        return -3;
    }

    if ( ma_device_start( &device ) != MA_SUCCESS )
    {
        printf( "Failed to start playback device.\n" );
        ma_device_uninit( &device );
        ma_decoder_uninit( &decoder );
        return -4;
    }


    /* Initialize the memory with ncurses session & start colors */

    /**
     * Inizializzo lo standar screen
     *
     *
     * @return
     */
    initscr();

    /**
     * Inizializzo la palette di colori
     *
     *
     * @return
     */
    initializeColors();

    do  /**< Inizia la sequenza di istruzioni da eseguire */
    {
        mod = menu(); /**< Prendo l'input da tastiera da parte dell'utente nel menu */
        if( mod == 0 )
            singlePlayer();     /**< Funzione single player */
        if( mod == 1 )
            multiPlayer();      /**< Funzione multiplayer */
        if( mod == 2 )
            CPU();              /**< Funzione playerVScpu */
    }
    while ( mod != 4 );

    paintExit();
    /* Deallocates memory and ends ncurses */
    /**
     * Refresh della schermata
     *
     *
     * @return
     */
    refresh();
    /**
     * Elimino la window e dealloco la memoria
     *
     *
     * @return
     */
    endwin();

    /**
     * Dealloco i devices della libreria musicale
     *
     * @param device
     *
     * @return
     */
    ma_device_uninit( &device );

    /**
     * Dealloco i decoder della libreria musicale
     *
     * @param decoder
     *
     * @return
     */
    ma_decoder_uninit( &decoder );

    return 0;
}
