﻿#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "audio_mgr.h"
#include "scene.h"
#include "doorbell.h"

static ITUSprite* audioPlayerStorageSprite;
static ITUBackground* audioPlayerStorageTypeBackground;
static ITURadioBox* audioPlayerSDRadioBox;
static ITURadioBox* audioPlayerUsbRadioBox;
static ITUScrollMediaFileListBox* audioPlayerScrollMediaFileListBox;
static ITUText* audioPlayerTimeText;
static ITUSprite* audioPlayerRepeatSprite;
static ITUCheckBox* audioPlayerRandomCheckBox;
static ITUCheckBox* audioPlayerPlayCheckBox;
static ITUSprite* audioPlayerVolSprite;
static ITUProgressBar* audioPlayerVolProgressBar;
static ITUTrackBar* audioPlayerVolTrackBar;

static bool audioPlayerPlaying;
static bool audioPlayerPlayingFinish;
static bool MgrReloadSBCRunning = true;

void AudioPlayerStop(void)
{

    if (audioPlayerPlaying)
    {
        AudioStop();
        ituCheckBoxSetChecked(audioPlayerPlayCheckBox, false);
        audioPlayerPlaying = false;
    }

    if(!MgrReloadSBCRunning){
        smtkAudioMgrReloadSBC(castor3snd_reinit_for_video_memo_play());
        MgrReloadSBCRunning = true ;
    }
    LinphoneResumeKeySound();
}

bool AudioPlayerSDInsertedOnCustom(ITUWidget* widget, char* param)
{
    if (!ituWidgetIsVisible(audioPlayerStorageSprite))
    {
        StorageType storageType = StorageGetCurrType();

        ituWidgetSetVisible(audioPlayerStorageSprite, true);
        ituWidgetSetVisible(audioPlayerScrollMediaFileListBox, true);
        ituSpriteGoto(audioPlayerStorageSprite, storageType);

        ituMediaFileListSetPath(&audioPlayerScrollMediaFileListBox->mflistbox, StorageGetDrivePath(storageType));
    }
    ituWidgetEnable(audioPlayerSDRadioBox);
    return true;
}

bool AudioPlayerSDRemovedOnCustom(ITUWidget* widget, char* param)
{
    if (audioPlayerStorageSprite->frame == STORAGE_SD)
    {
        StorageType storageType = StorageGetCurrType();

        if (audioPlayerPlaying)
        {
            AudioStop();
            //LinphoneResumeKeySound();
            audioPlayerPlaying = false;
        }

        ituCheckBoxSetChecked(audioPlayerPlayCheckBox, false);
        ituTextSetString(audioPlayerTimeText, NULL);

        if (storageType == STORAGE_NONE)
        {
            ituWidgetSetVisible(audioPlayerStorageSprite, false);
            ituWidgetSetVisible(audioPlayerScrollMediaFileListBox, false);
        }
        else
        {
            ituWidgetSetVisible(audioPlayerStorageSprite, true);
            ituWidgetSetVisible(audioPlayerScrollMediaFileListBox, true);
            ituSpriteGoto(audioPlayerStorageSprite, storageType);

            ituMediaFileListSetPath(&audioPlayerScrollMediaFileListBox->mflistbox, StorageGetDrivePath(storageType));
        }
    }
    ituWidgetDisable(audioPlayerSDRadioBox);
    return true;
}

bool AudioPlayerUsbInsertedOnCustom(ITUWidget* widget, char* param)
{
    if (!ituWidgetIsVisible(audioPlayerStorageSprite))
    {
        StorageType storageType = StorageGetCurrType();

        ituWidgetSetVisible(audioPlayerStorageSprite, true);
        ituWidgetSetVisible(audioPlayerScrollMediaFileListBox, true);
        ituSpriteGoto(audioPlayerStorageSprite, storageType);

        if (storageType == STORAGE_SD)
            ituRadioBoxSetChecked(audioPlayerSDRadioBox, true);
        else if (storageType == STORAGE_USB)
            ituRadioBoxSetChecked(audioPlayerUsbRadioBox, true);

        ituMediaFileListSetPath(&audioPlayerScrollMediaFileListBox->mflistbox, StorageGetDrivePath(storageType));
    }
    ituWidgetEnable(audioPlayerUsbRadioBox);
    return true;
}

bool AudioPlayerUsbRemovedOnCustom(ITUWidget* widget, char* param)
{
    if (audioPlayerStorageSprite->frame == STORAGE_USB)
    {
        StorageType storageType = StorageGetCurrType();

        if (audioPlayerPlaying)
        {
            AudioStop();
            //LinphoneResumeKeySound();
            audioPlayerPlaying = false;
        }

        ituCheckBoxSetChecked(audioPlayerPlayCheckBox, false);
        ituTextSetString(audioPlayerTimeText, NULL);

        if (storageType == STORAGE_NONE)
        {
            ituWidgetSetVisible(audioPlayerStorageSprite, false);
            ituWidgetSetVisible(audioPlayerScrollMediaFileListBox, false);
        }
        else
        {
            ituWidgetSetVisible(audioPlayerStorageSprite, true);
            ituWidgetSetVisible(audioPlayerScrollMediaFileListBox, true);
            ituSpriteGoto(audioPlayerStorageSprite, storageType);

            if (storageType == STORAGE_SD)
                ituRadioBoxSetChecked(audioPlayerSDRadioBox, true);
            else if (storageType == STORAGE_USB)
                ituRadioBoxSetChecked(audioPlayerUsbRadioBox, true);

            ituMediaFileListSetPath(&audioPlayerScrollMediaFileListBox->mflistbox, StorageGetDrivePath(storageType));
        }
    }
    ituWidgetDisable(audioPlayerUsbRadioBox);
    return true;
}

bool AudioPlayerStorageRadioBoxOnPress(ITUWidget* widget, char* param)
{
    StorageType storageType = StorageGetCurrType();

    if ((storageType == STORAGE_SD && widget == (ITUWidget*)audioPlayerSDRadioBox) ||
        (storageType == STORAGE_USB && widget == (ITUWidget*)audioPlayerUsbRadioBox))
        return false;

    if (audioPlayerPlaying)
    {
        AudioStop();
        //LinphoneResumeKeySound();
        audioPlayerPlaying = false;
    }
    
    ituCheckBoxSetChecked(audioPlayerPlayCheckBox, false);
    ituTextSetString(audioPlayerTimeText, NULL);

    if (widget == (ITUWidget*)audioPlayerSDRadioBox)
    {
        StorageSetCurrType(STORAGE_SD);
        ituSpriteGoto(audioPlayerStorageSprite, STORAGE_SD);
        ituMediaFileListSetPath(&audioPlayerScrollMediaFileListBox->mflistbox, StorageGetDrivePath(STORAGE_SD));
    }
    else if (widget == (ITUWidget*)audioPlayerUsbRadioBox)
    {
        StorageSetCurrType(STORAGE_USB);
        ituSpriteGoto(audioPlayerStorageSprite, STORAGE_USB);
        ituMediaFileListSetPath(&audioPlayerScrollMediaFileListBox->mflistbox, StorageGetDrivePath(STORAGE_USB));
    }
    return true;
}

bool AudioPlayerStorageTypeCheckBoxOnPress(ITUWidget* widget, char* param)
{
    if (ituWidgetIsVisible(audioPlayerStorageTypeBackground))
    {
        ituWidgetSetVisible(audioPlayerStorageTypeBackground, false);
        ituWidgetEnable(audioPlayerScrollMediaFileListBox);
    }
    else
    {
        ituWidgetSetVisible(audioPlayerStorageTypeBackground, true);
        ituWidgetDisable(audioPlayerScrollMediaFileListBox);
    }
    return true;
}

bool AudioPlayerScrollMediaFileListBoxOnSelection(ITUWidget* widget, char* param)
{
    return true;
}

bool AudioPlayerRepeatButtonOnPress(ITUWidget* widget, char* param)
{
    ITUMediaRepeatMode mode = audioPlayerRepeatSprite->frame;
    if (mode == ITU_MEDIA_REPEAT_ALL)
        mode = ITU_MEDIA_REPEAT_NONE;
    else
        mode++;

    audioPlayerScrollMediaFileListBox->mflistbox.repeatMode = mode;
    ituSpriteGoto(audioPlayerRepeatSprite, mode);

    return true;
}

bool AudioPlayerRandomCheckBoxOnPress(ITUWidget* widget, char* param)
{
    audioPlayerScrollMediaFileListBox->mflistbox.randomPlay = ituCheckBoxIsChecked(audioPlayerRandomCheckBox);
    return true;
}

static int AudioPlayerPlayCallback(int state)
{
    switch (state)
    {
    case AUDIOMGR_STATE_CALLBACK_PLAYING_FINISH:
        audioPlayerPlayingFinish = true;
        break;
    }
    return 0;
}

bool AudioPlayerLastButtonOnPress(ITUWidget* widget, char* param)
{
    ITUScrollText* item = ituMediaFileListPrev((ITUMediaFileListBox*)audioPlayerScrollMediaFileListBox);
    if (item)
    {
        char* filepath = (char*)ituWidgetGetCustomData(item);

        //LinphonePauseKeySound();
        if(!LinphoneGetWarnSoundPlaying()){
            AudioPlay(filepath, AudioPlayerPlayCallback);
            audioPlayerPlaying = true;
        }
        ituTextSetString(audioPlayerTimeText, NULL);
    }
    return true;
}

bool AudioPlayerPlayCheckBoxOnPress(ITUWidget* widget, char* param)
{
    if (ituCheckBoxIsChecked(audioPlayerPlayCheckBox))
    {
        ITUScrollText* item = ituMediaFileListPlay((ITUMediaFileListBox*)audioPlayerScrollMediaFileListBox);
        if (item)
        {
            char* filepath = (char*)ituWidgetGetCustomData(item);

            //LinphonePauseKeySound();
            if(!LinphoneGetWarnSoundPlaying()){
                AudioPlay(filepath, AudioPlayerPlayCallback);
                audioPlayerPlaying = true;
            }
        }
        else
        {
            ituCheckBoxSetChecked(audioPlayerPlayCheckBox, false);
        }
    }
    else
    {
        if (audioPlayerPlaying)
        {
            AudioStop();
            //LinphoneResumeKeySound();
            audioPlayerPlaying = false;
        }
    }
    ituTextSetString(audioPlayerTimeText, NULL);
    return true;
}

bool AudioPlayerNextButtonOnPress(ITUWidget* widget, char* param)
{
    ITUScrollText* item = ituMediaFileListNext((ITUMediaFileListBox*)audioPlayerScrollMediaFileListBox);
    if (item)
    {
        char* filepath = (char*)ituWidgetGetCustomData(item);

        //LinphonePauseKeySound();
        if(!LinphoneGetWarnSoundPlaying()){
            AudioPlay(filepath, AudioPlayerPlayCallback);
            audioPlayerPlaying = true;
        }
        ituTextSetString(audioPlayerTimeText, NULL);
        
    }
    return true;
}

bool AudioPlayerVolTrackBarOnChanged(ITUWidget* widget, char* param)
{
    int vol;

    if (!audioPlayerVolSprite || LinphoneGetWarnSoundPlaying())
        return false;

    vol = atoi(param);

    if (vol > 0)
    {
        ituSpriteGoto(audioPlayerVolSprite, 1);
    }
    else
    {
        ituSpriteGoto(audioPlayerVolSprite, 0);
    }

    AudioSetVolume(vol);
    return true;
}

bool AudioPlayerOnTimer(ITUWidget* widget, char* param)
{
    if (audioPlayerPlaying)
    {
        int h, m, s;
        char buf[32];
        
        s = smtkAudioMgrGetTime() / 1000;
        m = s / 60;
        h = m / 60;
        s -= m * 60;
        m -= h * 60;

        if (h > 0)
            sprintf(buf, "%02d:%02d:%02d", h, m, s);
        else
            sprintf(buf, "%02d:%02d", m, s);

        ituTextSetString(audioPlayerTimeText, buf);

        ScreenSaverRefresh();
    }
	if (audioPlayerPlayingFinish)
	{
		if (!LinphoneGetWarnSoundPlaying())
		{
			ITUScrollText* item = ituMediaFileListNext((ITUMediaFileListBox*)audioPlayerScrollMediaFileListBox);
            if (item)
            {
                char* filepath = (char*)ituWidgetGetCustomData(item);

                //LinphonePauseKeySound();
                AudioPlay(filepath, AudioPlayerPlayCallback);
            }
            else
            {
                //LinphoneResumeKeySound();
                ituCheckBoxSetChecked(audioPlayerPlayCheckBox, false);
                audioPlayerPlaying = false;
            }
            ituTextSetString(audioPlayerTimeText, NULL);
		}
		audioPlayerPlayingFinish = false;
	}
    return true;
}

bool AudioPlayerOnEnter(ITUWidget* widget, char* param)
{
    StorageType storageType;
    int vol;
    LinphonePauseKeySound();
    
    if(!LinphoneGetWarnSoundPlaying() && MgrReloadSBCRunning){
        //smtkAudioMgrUnloadSBC();
        castor3snd_deinit_for_video_memo_play();
        MgrReloadSBCRunning = false ;
    }
    
    if (!audioPlayerStorageSprite)
    {
        audioPlayerStorageSprite = ituSceneFindWidget(&theScene, "audioPlayerStorageSprite");
        assert(audioPlayerStorageSprite);

        audioPlayerStorageTypeBackground = ituSceneFindWidget(&theScene, "audioPlayerStorageTypeBackground");
        assert(audioPlayerStorageTypeBackground);

        audioPlayerSDRadioBox = ituSceneFindWidget(&theScene, "audioPlayerSDRadioBox");
        assert(audioPlayerSDRadioBox);

        audioPlayerUsbRadioBox = ituSceneFindWidget(&theScene, "audioPlayerUsbRadioBox");
        assert(audioPlayerUsbRadioBox);

        audioPlayerScrollMediaFileListBox = ituSceneFindWidget(&theScene, "audioPlayerScrollMediaFileListBox");
        assert(audioPlayerScrollMediaFileListBox);

        audioPlayerTimeText = ituSceneFindWidget(&theScene, "audioPlayerTimeText");
        assert(audioPlayerTimeText);

        audioPlayerRepeatSprite = ituSceneFindWidget(&theScene, "audioPlayerRepeatSprite");
        assert(audioPlayerRepeatSprite);

        audioPlayerRandomCheckBox = ituSceneFindWidget(&theScene, "audioPlayerRandomCheckBox");
        assert(audioPlayerRandomCheckBox);

        audioPlayerPlayCheckBox = ituSceneFindWidget(&theScene, "audioPlayerPlayCheckBox");
        assert(audioPlayerPlayCheckBox);

        audioPlayerVolSprite = ituSceneFindWidget(&theScene, "audioPlayerVolSprite");
        assert(audioPlayerVolSprite);

        audioPlayerVolProgressBar = ituSceneFindWidget(&theScene, "audioPlayerVolProgressBar");
        assert(audioPlayerVolProgressBar);

        audioPlayerVolTrackBar = ituSceneFindWidget(&theScene, "audioPlayerVolTrackBar");
        assert(audioPlayerVolTrackBar);
    }
    storageType = StorageGetCurrType();

    if (storageType == STORAGE_NONE)
    {
        ituWidgetSetVisible(audioPlayerStorageSprite, false);
        ituWidgetSetVisible(audioPlayerScrollMediaFileListBox, false);
    }
    else
    {
        ituWidgetSetVisible(audioPlayerStorageSprite, true);
        ituWidgetSetVisible(audioPlayerScrollMediaFileListBox, true);
        ituSpriteGoto(audioPlayerStorageSprite, storageType);

        if (storageType == STORAGE_SD)
            ituRadioBoxSetChecked(audioPlayerSDRadioBox, true);
        else if (storageType == STORAGE_USB)
            ituRadioBoxSetChecked(audioPlayerUsbRadioBox, true);

        if (StorageGetDrivePath(STORAGE_SD))
            ituWidgetEnable(audioPlayerSDRadioBox);
        else
            ituWidgetDisable(audioPlayerSDRadioBox);

        if (StorageGetDrivePath(STORAGE_USB))
            ituWidgetEnable(audioPlayerUsbRadioBox);
        else
            ituWidgetDisable(audioPlayerUsbRadioBox);

        ituMediaFileListSetPath(&audioPlayerScrollMediaFileListBox->mflistbox, StorageGetDrivePath(storageType));
    }
    if(LinphoneGetWarnSoundPlaying())
        vol = LinphoneGetWarnLevel();
    else
        vol = AudioGetVolume();
    ituSpriteGoto(audioPlayerVolSprite, vol > 0 ? 1 : 0);
    ituProgressBarSetValue(audioPlayerVolProgressBar, vol);
    ituTrackBarSetValue(audioPlayerVolTrackBar, vol);
    ituTextSetString(audioPlayerTimeText, NULL);
    AudioSetVolume(vol);
    audioPlayerPlaying = false;
    audioPlayerPlayingFinish = false;
    LinphoneStartMediaAudioPlay();

    return true;
}

bool AudioPlayerOnLeave(ITUWidget* widget, char* param)
{
    LinphoneStopMediaAudioPlay();
    if (audioPlayerPlaying)
    {
        AudioStop();
        ituCheckBoxSetChecked(audioPlayerPlayCheckBox, false);
        audioPlayerPlaying = false;
    }

    if(!MgrReloadSBCRunning){
        smtkAudioMgrReloadSBC(castor3snd_reinit_for_video_memo_play());
        MgrReloadSBCRunning = true ;
    }
    
    LinphoneResumeKeySound();
	SceneContinuePlayGuardAlarm();

    return true;
}

void AudioPlayerReset(void)
{
    audioPlayerStorageSprite = NULL;
}
