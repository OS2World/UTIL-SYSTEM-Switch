/*----------------------------------------------------------------------*
 * Switch                                                               *
 *                                                                      *
 * This program allows you to modify the 'Jump' toggle in the Session   *
 * Task List kept by OS/2.                                              *
 *                                                                      *
 * I wrote this primarily because I wanted to be able to confine the    *
 * Round Robin effect of the Alt+Esc keys sequence to certain sessions. *
 *                                                                      *
 * This program is public domain.  All I ask is that if you modify this *
 * program and make it available to the public that you do *not* modify *
 * this header nor remove my copyright logo from the program, but only  *
 * extend the history modification list.  Also-- please send me a       *
 * copy!!                                                               *
 *                                                                      *
 * Modification History                                                 *
 *                                                                      *
 * 25-Jun-1991  Dave Fisher, 1:170/110@fidonet                          *
 *    1.00 : Original version release.                                  *
 *                                                                      *
 *----------------------------------------------------------------------*/

#define INCL_WINSWITCHLIST
#define INCL_WINPROGRAMLIST

#include <os2.h>

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define VERSION "1.00"

void display_tasklist( SWBLOCK *tasklist, int numitems );
static void logo( void );

void main( int argc, char **argv, char **envp )
{
    HAB hab;
    int numitems;
    int status;
    SWBLOCK *tasklist;
    int tasklistsize;
    char optionstr[80];
    int num;

    logo();

    hab = WinInitialize(0);     /* get anchor block handle */

    numitems = WinQuerySwitchList( hab, (SWBLOCK *) 0, 0 );

    if ( numitems <= 0 )
        {
        printf("No items found in Task List\n");
        exit(1);
        }

    tasklistsize = (numitems * sizeof(SWENTRY)) + sizeof(HSWITCH);

    tasklist = (SWBLOCK *) malloc( tasklistsize );

    if ( !tasklist )
        {
        printf("Could not memory allocate task list\n");
        exit(1);
        }

    status = WinQuerySwitchList( hab, tasklist, tasklistsize );

    if ( !status )
        {
        printf("Could not query Task List\n");
        exit(1);
        }

    while( 1 )
        {
        display_tasklist( tasklist, numitems );

        printf("\n\nPress number to toggle jump status (Enter=quit): ");
        gets( optionstr );

        num = atoi(optionstr);

        if ( num <= 0 )
            break;

        if ( num > numitems )
            continue;

        num--;

        switch( tasklist->aswentry[num].swctl.fbJump )
            {
            case SWL_JUMPABLE :
                tasklist->aswentry[num].swctl.fbJump = SWL_NOTJUMPABLE;
                break;
            case SWL_NOTJUMPABLE :
                tasklist->aswentry[num].swctl.fbJump = SWL_JUMPABLE;
                break;
            }

        status = WinChangeSwitchEntry( tasklist->aswentry[num].hswitch,
                                       &(tasklist->aswentry[num].swctl) );

        if ( status != 0 )
            {
            printf("Could not modify entry '%s'\n",
                            tasklist->aswentry[num].swctl.szSwtitle );
            }
        }

    WinTerminate(hab);

    exit(0);
}


void display_tasklist( SWBLOCK* tasklist, int numitems )
{
    register int i;
    char jumpstr[40];

    printf("\n");

    for( i=0; i < numitems; i++ )
        {
        switch( tasklist->aswentry[i].swctl.fbJump )
            {
            case SWL_JUMPABLE :
                strcpy( jumpstr, "Jumpable" );
                break;
            case SWL_NOTJUMPABLE :
                strcpy( jumpstr, "Not Jumpable" );
                break;
            default :
                strcpy( jumpstr, "???" );
                break;
            }

        printf("[%02d] [%12s] %s\n", i+1, jumpstr,
            tasklist->aswentry[i].swctl.szSwtitle );
        }
}

static void logo( void )
{
    printf( "\nSwitch - Session Task List Utility, Version %s\n", VERSION);
    printf( "Copyright 1991 by Dave Fisher, 1:170/110@fidonet.  All rights reserved.\n");
}
