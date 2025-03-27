#include "manual_play.h"
#include <stdio.h>

void select_move_manuel(MoveData* mymove){
    ClaimRouteMove claim_route;

    printf("choose action :\n1 | claim route\n2 | draw blind card\n3 | draw card\n4 | draw objectives\n5 | choose objectives\n");
    scanf("%d",(int*)&mymove->action);
    if(mymove->action == 1){
        printf("choose route : from where ?\n");
        scanf("%d",(int*)&claim_route.from);
        printf("choose route : to where ?\n");
        scanf("%d",(int*)&claim_route.to);
        printf("choose route : what color ?\n");
        scanf("%d",(int*)&claim_route.color);
        printf("choose route : nb locomotives ?\n");
        scanf("%d",(int*)&claim_route.nbLocomotives);
        mymove->claimRoute = claim_route;
    }
    if(mymove->action == 3){
        printf("draw card : what color ?\n");
        scanf("%d",(int*)&mymove->drawCard);
    }
    if(mymove->action == 5){
        printf("draw objectives : which ones ?\nN°1 : 1 Yes | 0 No\n");
        scanf("%d",(int*)&mymove->chooseObjectives[0]);
        printf("N°2 : 1 Yes | 0 No\n");
        scanf("%d",(int*)&mymove->chooseObjectives[1]);
        printf("N°3 : 1 Yes | 0 No\n");
        scanf("%d",(int*)&mymove->chooseObjectives[2]);
    }
    return;
}