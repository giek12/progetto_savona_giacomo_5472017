#include "MyHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"

void AMyHUD::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("MyHUD BeginPlay eseguito"));

}

void AMyHUD::DrawHUD()
{
    Super::DrawHUD();

    UFont* Font = LoadObject<UFont>(nullptr, TEXT("/Engine/EngineFonts/Roboto.Roboto"));
    FVector2D TextSize = FVector2D(10.0f, 10.0f);

    Canvas->StrLen(Font, HUDMessage, TextSize.X, TextSize.Y);   //messaggio di turno / vittoria / sconfitta
    Canvas->StrLen(Font, LPHPS, TextSize.X, TextSize.Y);        //vita dello sniper player
    Canvas->StrLen(Font, LPHPB, TextSize.X, TextSize.Y);        //vita del brawler player
    Canvas->StrLen(Font, LPAIS, TextSize.X, TextSize.Y);        //vita dello sniper ai
    Canvas->StrLen(Font, LPAIB, TextSize.X, TextSize.Y);        //vita del brawler ai

    float X = (Canvas->SizeX - TextSize.X) * 0.5f;          //turno
    float Y = Canvas->SizeY * 0.0f;

    float XOffset = Canvas->SizeX * 0.8f;

    float X1 = XOffset;                         // HP S
    float Y1 = Canvas->SizeY * 0.0f;

    float X2 = XOffset;                         // HP B
    float Y2 = (Canvas->SizeY - TextSize.Y) * 0.25f;

    float X3 = XOffset;                         // AI S
    float Y3 = (Canvas->SizeY - TextSize.Y) * 0.75f;

    float X4 = XOffset;                         // AI B
    float Y4 = (Canvas->SizeY - 2 * TextSize.Y);


    float ScaleFactor = FMath::Clamp(Canvas->SizeX / 1920.0f, 0.5f, 1.5f);
    ScaleFactor *= 3.0f;
    FLinearColor TextColor = FLinearColor::White;

    FCanvasTextItem NewText(FVector2D(X, Y), FText::FromString(HUDMessage), Font, TextColor);
    NewText.Scale = FVector2D(ScaleFactor, ScaleFactor);

    FCanvasTextItem NewText1(FVector2D(X1, Y1), FText::FromString(LPHPS), Font, TextColor);
    NewText1.Scale = FVector2D(ScaleFactor, ScaleFactor);

    FCanvasTextItem NewText2(FVector2D(X2, Y2), FText::FromString(LPHPB), Font, TextColor);
    NewText2.Scale = FVector2D(ScaleFactor, ScaleFactor);

    FCanvasTextItem NewText3(FVector2D(X3, Y3), FText::FromString(LPAIS), Font, TextColor);
    NewText3.Scale = FVector2D(ScaleFactor, ScaleFactor);

    FCanvasTextItem NewText4(FVector2D(X4, Y4), FText::FromString(LPAIB), Font, TextColor);
    NewText4.Scale = FVector2D(ScaleFactor, ScaleFactor);

    //impostazioni testi

    Canvas->DrawItem(NewText);
    Canvas->DrawItem(NewText1);
    Canvas->DrawItem(NewText2);
    Canvas->DrawItem(NewText3);
    Canvas->DrawItem(NewText4);

    //stampa a schermo testi
}

void AMyHUD::SetHUDMessage(const FString& NewMessage)
{
    HUDMessage = NewMessage;
}

void AMyHUD::SetHUDLPHPS(int LP)
{
    LPHPS = FString::Printf(TEXT("Punti vita HP S: %d"), LP);
}

void AMyHUD::SetHUDLPHPB(int LP)
{
    LPHPB = FString::Printf(TEXT("Punti vita HP B: %d"), LP);
}

void AMyHUD::SetHUDLPAIS(int LP)
{
    LPAIS = FString::Printf(TEXT("Punti vita AI S: %d"), LP);
}

void AMyHUD::SetHUDLPAIB(int LP)
{
    LPAIB = FString::Printf(TEXT("Punti vita AI B: %d"), LP);
}