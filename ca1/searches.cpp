/*void playGame() {*/
/*    while (true) {*/
/*        // Display the current state*/
/*        printf("\nCurrent State:\n");*/
/*        printf("Value of wrong: %d\n", wrong);*/
/*        print();*/
        /*printf("holex: %d, holey: %d\n", holex, holey);*/
        /*for (int y = 0; y < size*2 - 1; y += 1) {*/
        /*    for (int x = 0; x < size*2 - 1; x += 1) {*/
        /*        printf("[%d %d] (%d) = %c ((%c))\n", x, y, y*size + x, get(x, y), squares[y*size + x]);*/
        /*    }*/
        /*}*/
/**/
/*        // Show legal moves*/
/*        vector<Pos> legalMoves = getLegalMoves();*/
/*        if (legalMoves.empty()) {*/
/*            printf("No legal moves available. Game over.\n");*/
/*            break;*/
/*        }*/
/**/
/*        printf("Legal Moves:\n");*/
/*        for (size_t i = 0; i < legalMoves.size(); ++i) {*/
/*            printf("%zu: (%d, %d)\n", i, legalMoves[i].x, legalMoves[i].y);*/
/*        }*/
/**/
/*        // Ask the user to make a move*/
/*        size_t moveIndex;*/
/*        printf("Enter the index of the move you want to make: ");*/
/*        cin >> moveIndex;*/
/**/
/*        if (moveIndex >= legalMoves.size()) {*/
/*            printf("Invalid move index. Please try again.\n");*/
/*            continue;*/
/*        }*/
/**/
/*        // Make the move*/
/*        makeMove(legalMoves[moveIndex].x, legalMoves[moveIndex].y);*/
/*    }*/
/**/
