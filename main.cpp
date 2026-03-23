#include <raylib.h>
#include <vector>
#include <string>
#include <queue>

using namespace std;

float timerForWordInstanceInterval = 0.0f;
string wordsArray[] = {"Inny","Love","Claud","Moon","Boring game","Milk tea"};
queue<string> wordsQueue;
vector<Vector2> positions = {{200, 50}, {400, 50}, {600, 50}};
int positionCount = 0;

// Words~ don't come easy to me.
struct Word{
    string word = "Blank";
    Vector2 position;
    float timerForDropInterval = 0.0f;

    Word(){
        word = wordsQueue.front();
        wordsQueue.pop();
        position = positions[positionCount];
    }
    
    void UpdateAndDraw(){
        if(timerForDropInterval >= 0.5f){
            position.y += 10;
            timerForDropInterval = 0.0f;
        }
        timerForDropInterval += GetFrameTime();
        DrawRectangle(position.x-5, position.y-3, 50, 30, LIGHTGRAY);
        DrawText(word.c_str(), position.x, position.y, 20, BLACK);
    }
};

vector<Word> words;
string input;
int ch;
float inputPosX;
float wrongTimer = 0.0f;
bool gameOver = false;
bool fail = false;

int main(){
    InitWindow(800, 450, "Word Rain");
    SetTargetFPS(60);

    for(int i=0; i < sizeof(wordsArray)/sizeof(wordsArray[0]); i++){
        wordsQueue.push(wordsArray[i]);
    }

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);

        if(!gameOver){
            
            DrawText(TextFormat("Words: %i", words.size()), 10, 10, 20, BLACK);
    
            while((ch=GetCharPressed()) > 0){
                input += (char)ch;
            }
            
            inputPosX= (float)GetScreenWidth()/2-MeasureText(input.c_str(), 20)/2;
            DrawText(input.c_str(), inputPosX, 400, 20, BLACK);
    
            if(IsKeyPressed(KEY_BACKSPACE) && !input.empty()){
                input.erase(input.end()-1);
            }
            if(IsKeyPressed(KEY_ENTER) && !input.empty()){
                if(any_of(words.begin(), words.end(), [&](Word& w){ return input == w.word;})){
                    words.erase(remove_if(words.begin(), words.end(), [&](Word& w){return w.word == input;}), words.end());
                }
                else
                    wrongTimer = 1.0f;
                    
                input = "";
            }
    
            if(wrongTimer > 0){
                DrawText("Wrong!", 400, 225, 20, RED);
                wrongTimer -= GetFrameTime();
            }
    
            timerForWordInstanceInterval += GetFrameTime();
    
            if(timerForWordInstanceInterval > 1.0f && !wordsQueue.empty()){
                Word word1;
                words.push_back(word1);
                positionCount++;
                if(positionCount == positions.size())
                    positionCount = 0;
                timerForWordInstanceInterval = 0.0f;
            }

            // 성공 체크
            if(words.empty() && wordsQueue.empty()){
                gameOver = true;
            }
    
            for(auto& word: words){
                word.UpdateAndDraw();
                if(word.position.y > 430){
                    gameOver = true;
                    fail = true;
                }
            }
        }

        if(gameOver){
            string sentence = (fail) ? "Fail" : "Success!";
            DrawText(sentence.c_str(), 350, 225, 40, GREEN);
        }

        EndDrawing();
    }
    CloseWindow();
}