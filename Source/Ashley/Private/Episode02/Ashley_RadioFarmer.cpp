// Ashley


#include "Episode02/Ashley_RadioFarmer.h"

#include "Components/AudioComponent.h"
#include "Characters/Players/Ashley_CharacterPlayer.h"

#include "DebugHelper.h"
#include <Managers/Ashley_WidgetSystem.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>


AAshley_RadioFarmer::AAshley_RadioFarmer()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    // AudioComponent dedicato al suono di switch
    AudioComponentSwitch = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponentSwitch"));
    AudioComponentSwitch->SetupAttachment(RootComponent);

    // Configuro il layer per essere interagibile dal player
    MeshBase->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
}

void AAshley_RadioFarmer::BeginPlay()
{
    Super::BeginPlay();

    // Per ogni "canale", creo un AudioComponent dedicato
    int index = 0;
    for (auto& message : RadioMessages)
    {
        auto AudioComponentRadio = NewObject<UAudioComponent>(this);
        AudioComponentRadio->bAutoActivate = false;
        AudioComponentRadio->RegisterComponent();
        AudioComponentRadio->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        AudioComponentRadio->SetSound(message.Sound);
        AudioComponentsRadio.Add(AudioComponentRadio);
        index++;
    }

    ChannelsNumber = AudioComponentsRadio.Num() + 1;
    CurrentChannel = 1;

    for (auto& audioComponent : AudioComponentsRadio)
    {
        audioComponent->Play();
        audioComponent->SetFloatParameter("Volume", 0);
        RadioChannelsPlayback.Add(0);
    }

    AudioComponentsRadio[0]->SetFloatParameter("Volume", 1);

    Player = Cast<AAshley_CharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
}

void AAshley_RadioFarmer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!Player) return;

    if (CurrentChannel == 0) return;

    for (int i = 0; i < RadioChannelsPlayback.Num(); i++)
    {
        RadioChannelsPlayback[i] = 
            FMath::Fmod(
                (RadioChannelsPlayback[i] + DeltaTime),
                (RadioMessages[i].SoundDuration));
    }

    auto Dialogue = CalculateCurrentSubtitle();

    // Modifico il volume in base alla distanza del player, in quanto
    //  non posso affidarmi all'attenuazione (data dalla distanza dalla CAMERA)
    auto Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
    CurrentVolume = 1.0f - FMath::Lerp(0.f, 1.0f, FMath::Clamp(Distance / MaxDistance, 0.0f, 1.0f));

    //Debug::Print(FString::Printf(TEXT("Distance: %f ALPHA: %f"), Distance, FMath::Clamp(Distance / 500, 0, 1)));

    AudioComponentsRadio[CurrentChannel - 1]->SetFloatParameter("Volume", CurrentVolume);

    if (Dialogue != CurrentDialogue && Dialogue != FString())
    {
        CurrentDialogue = Dialogue;
        if (bIsShowingDialogue) ShowDialogue(CurrentDialogue);
    }
}

FString AAshley_RadioFarmer::CalculateCurrentSubtitle()
{
    auto& SubtitleCues = RadioMessages[CurrentChannel - 1].Subtitles;
    float PlaybackTime = RadioChannelsPlayback[CurrentChannel - 1];

    for (int32 i = SubtitleCues.Num() - 1; i >= 0; --i)
    {
        if (PlaybackTime >= SubtitleCues[i].TimestampStart)
        {
            //auto& ss = SubtitleCues[i].Subtitle;
            //Debug::Print(FString::Printf(TEXT("Found dialogue %s"), *ss));
            return SubtitleCues[i].Subtitle;
        }
    }

    return FString();
}

void AAshley_RadioFarmer::ShowDialogue(FString Dialogue)
{
    UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->SpawnInfoText(Dialogue, - 1);
}

void AAshley_RadioFarmer::HideDialogue()
{
    UGameplayStatics::GetGameInstance(this)->GetSubsystem<UAshley_WidgetSystem>()->HideInfoText();
}

void AAshley_RadioFarmer::Interact_Implementation()
{
    AudioComponentSwitch->Play();

    if (CurrentChannel > 0)
        AudioComponentsRadio[CurrentChannel - 1]->SetFloatParameter("Volume", 0);
    else
        ShowDialogue(CurrentDialogue);

    CurrentChannel = (CurrentChannel + 1) % (ChannelsNumber - 1); // Riduco di 1 per non poter settare il canale "horror" con l'interazione

    // Se sono passato ad un canale valido, attivo il volume
    if (CurrentChannel > 0)
        AudioComponentsRadio[CurrentChannel - 1]->SetFloatParameter("Volume", CurrentVolume);
    // Altrimenti nascondo i sottotitoli
    else
        HideDialogue();

    //Debug::Print(FString::Printf(TEXT("Current Channel %d"), CurrentChannel));
}

void AAshley_RadioFarmer::SwitchToHauntedChannel()
{
    if (CurrentChannel > 0)
        AudioComponentsRadio[CurrentChannel - 1]->SetFloatParameter("Volume", 0);
    else
        ShowDialogue(CurrentDialogue);

    CurrentChannel = ChannelsNumber - 1;

    AudioComponentsRadio[CurrentChannel - 1]->SetFloatParameter("Volume", CurrentVolume);
}

//void AAshley_RadioFarmer::NotifyActorBeginOverlap(AActor* OtherActor)
//{
//    Super::NotifyActorBeginOverlap(OtherActor);
//
//    if (bIgnoreCollision) return;
//
//    // show dialogue
//    bIsShowingDialogue = true;
//    if (CurrentChannel > 0) ShowDialogue(CurrentDialogue);
//}

void AAshley_RadioFarmer::OnBoxInteractOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Super::OnBoxInteractOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

    if (bIgnoreCollision) return;

    // show dialogue
    bIsShowingDialogue = true;
    if (CurrentChannel > 0) ShowDialogue(CurrentDialogue);
}

//void AAshley_RadioFarmer::NotifyActorEndOverlap(AActor* OtherActor)
//{
//    Super::NotifyActorEndOverlap(OtherActor);
//
//    if (bIgnoreCollision) return;
//
//    // hide dialogue
//    bIsShowingDialogue = false;
//    HideDialogue();
//}

void AAshley_RadioFarmer::OnBoxInteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnBoxInteractEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

    if (bIgnoreCollision) return;

    // hide dialogue
    bIsShowingDialogue = false;
    HideDialogue();
}

FString AAshley_RadioFarmer::GetInteractMessage() const
{
    if (CurrentChannel == 0)
        return "Turn on";
    else if (CurrentChannel == ChannelsNumber - 1)
        return "Turn off";
    else
        return "Change channel";
}
