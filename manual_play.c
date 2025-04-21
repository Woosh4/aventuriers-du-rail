#include "manual_play.h"
#include <stdio.h>

void select_move_manuel(Player_Info* info){

    printf("choose action :\n1 | claim route\n2 | draw blind card\n3 | draw card\n4 | draw objectives\n5 | choose objectives\n");
    scanf("%d",(int*)&info->movedata->action);
    if(info->movedata->action == 1){
        printf("choose route : from where ?\n");
        scanf("%d",&info->movedata->claimRoute.from);
        printf("choose route : to where ?\n");
        scanf("%d",&info->movedata->claimRoute.to);
        printf("choose route : what color ?\n");
        scanf("%d",(int*)&info->movedata->claimRoute.color);
        printf("choose route : nb locomotives ?\n");
        scanf("%d",&info->movedata->claimRoute.nbLocomotives);
    }
    if(info->movedata->action == 3){
        printf("draw card : what color ?\n");
        scanf("%d",(int*)&info->movedata->drawCard);
    }
    if(info->movedata->action == 5){
        printf("draw objectives : which ones ?\nN°1 : 1 Yes | 0 No\n");
        scanf("%d",(int*)&info->movedata->chooseObjectives[0]);
        printf("N°2 : 1 Yes | 0 No\n");
        scanf("%d",(int*)&info->movedata->chooseObjectives[1]);
        printf("N°3 : 1 Yes | 0 No\n");
        scanf("%d",(int*)&info->movedata->chooseObjectives[2]);
    }
    return;
}