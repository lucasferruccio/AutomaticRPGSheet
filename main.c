#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <stdint.h>
#include "raylib.h"
#include "RollDice.h"

#define MAX_INPUT_CHARS 10
#define NUM_FRAMES 3

typedef uint8_t BYTE;

void split_info(void);

char dices[100], resultado1[10];

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "RollDices");
    
    
    
    //Open the images
    Texture2D DiceImage = LoadTexture("Resources/d20.png");
    Texture2D RollButton = LoadTexture("Resources/Button.png");
    Texture2D Logo = LoadTexture("Resources/name.png");
    Texture2D madeby = LoadTexture("Resources/bylucas.png");
    
    //Define the part of the image that`s is going to apears
    float frameHeight = (float)RollButton.height/NUM_FRAMES; //Separa a parte da foto que vai aparecer (altura)
    Rectangle RollButtonRec = {0 , 0, (float)RollButton.width, frameHeight}; //O retangulo da foto que vai aprecer
    
    //Define the limits of the rectangle
    Rectangle RollButtonLim = {70 , 125, 100, 50};
    
    int RollButtonState = 0;
    bool RollButtonAction = false;
    
    Rectangle DiceWriting = {70, 70, 250, 50};
    bool MouseOnText = false;

    //Dice results
    Rectangle ListOfDices = {70, 180, 400, 250};

    int letterCount = 0;
    
    SetTargetFPS(10);
    
    char Dice[MAX_INPUT_CHARS];
    memset(Dice, 0, 10);
    while (!WindowShouldClose()) 
    {
        //Button Mechanic
        RollButtonAction = false;
        if (CheckCollisionPointRec(GetMousePosition(), DiceWriting)) 
        {
            MouseOnText = true;
        }
        else
        {
            MouseOnText = false;
        }
        
        if (CheckCollisionPointRec(GetMousePosition(), RollButtonLim))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                memset(resultado1, 0, 10);
                memset(dices, 0, 100);
                dice_main(Dice);
                split_info();
                RollButtonState = 2;
            }
            else
            {
                RollButtonState = 1;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                
                RollButtonAction = true;
            }
        }
        else
        {
            RollButtonState = 0;
        }
    
        RollButtonRec.y = RollButtonState * frameHeight;
        
        if(MouseOnText)
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            
            //Pick the character typed by the user
            int Char_Input = GetCharPressed();
            
            //For dealing with the return 0 by the function
            while(Char_Input > 0)
            {
                //To pick only character that are needed for roolling the dices
                if ((Char_Input >= 32) && (Char_Input <= 125) && (letterCount < MAX_INPUT_CHARS))
                {
                    Dice[letterCount] = (char)Char_Input;
                    Dice[letterCount + 1] = '\0';
                    letterCount++;
                }
                
                Char_Input = GetCharPressed();
            }
            Char_Input = GetCharPressed();
            //To erase the characters
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0)
                {
                    letterCount = 0;
                }
                Dice[letterCount] = '\0';
            }
        }
        else
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
        
        //Drawing the HUD
        BeginDrawing();
        
            ClearBackground(WHITE);
        
            //The Dice with the results inside
            DrawTexture(DiceImage, screenWidth/2 + 100, screenHeight/2 - 150, WHITE);
            DrawText(resultado1, screenWidth/2 + 191, screenHeight/2 - 60, 60, BLACK);
        
            //The input bars, recive the dice value
            DrawRectangleRec(DiceWriting, RAYWHITE);
            DrawRectangleLines(DiceWriting.x, DiceWriting.y, DiceWriting.width, DiceWriting.height, DARKGRAY);
            DrawText(Dice, (int)DiceWriting.x + 5, (int)DiceWriting.y + 10, 30, BLACK);
        
            //The Roll Button
            DrawTextureRec(RollButton, RollButtonRec, (Vector2){ RollButtonLim.x, RollButtonLim.y}, WHITE);
            
            //The dice list
            DrawRectangleRec(ListOfDices, RAYWHITE);
            DrawRectangleLines(ListOfDices.x, ListOfDices.y, ListOfDices.width, ListOfDices.height, DARKGRAY);
            DrawText(dices, (int)ListOfDices.x + 5, (int)ListOfDices.y + 5, 20, BLACK);
            
            //Name and made by
            DrawTexture(Logo, screenWidth/2 - Logo.width/2, 0, WHITE);
            DrawTexture(madeby, screenWidth - madeby.width - 10, screenHeight - madeby.height, WHITE);

        EndDrawing();
    }
    UnloadTexture(DiceImage);
    UnloadTexture(RollButton);
    CloseWindow();

    return 0;
}

void split_info(void)
{
    int count = 0, dices_count = 0, result_count = 0, div = 0;
    FILE *dados = fopen("dados.txt", "r");

    BYTE byte1[1];
    while (fread(&byte1, sizeof(BYTE), 1, dados))
    {
        if (count == 1)
        {
            if ((div%45) == 0)
            {
                dices[dices_count] = '\n';
                dices_count++;
                dices[dices_count] = *byte1;
                dices_count++;
            }
            else
            {
                dices[dices_count] = *byte1;
                dices_count++;
            }
        }
        if (*byte1 == '\n')
        {
            count++;
        }
        div++;
    }
    fclose(dados);
    FILE *resultado = fopen("resultado.txt", "r");
    BYTE byte2[1];
    while (fread(&byte2, sizeof(BYTE), 1, dados))
    {
        resultado1[result_count] = *byte2;
        result_count++;
    }
}



bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 32) && (key <= 126)) keyPressed = true;

    return keyPressed;
}
