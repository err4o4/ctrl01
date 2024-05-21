#pragma once
#include <string>
#include <functional>

#define RemoteCommandIDPlay 0
#define RemoteCommandIDPause 1
#define RemoteCommandIDTogglePlayPause 2
#define RemoteCommandIDNextTrack 3
#define RemoteCommandIDPreviousTrack 4
#define RemoteCommandIDVolumeUp 5
#define RemoteCommandIDVolumeDown 6
#define RemoteCommandIDAdvanceRepeatMode 7
#define RemoteCommandIDAdvanceShuffleMode 8
#define RemoteCommandIDSkipForward 9
#define RemoteCommandIDSkipBackward 10
#define RemoteCommandIDLikeTrack 11
#define RemoteCommandIDDislikeTrack 12
#define RemoteCommandIDBookmarkTrack 13

class BLEClient;

namespace AppleMediaService {
  struct MediaInformation {
    enum class PlaybackState: uint8_t {
      Paused = 0,
      Playing = 1,
      Rewinding = 2,
      FastForwarding = 3
    };

    enum class ShuffleMode: uint8_t {
      Off = 0,
      One = 1,
      All = 2
    };

    enum class RepeatMode: uint8_t {
      Off = 0,
      One = 1,
      All = 2
    };
    // PlayerAttributeID
    std::string mPlayerName;
    PlaybackState mPlaybackState;
    float mPlaybackRate;
    float mElapsedTime;
    float mVolume;
    // QueueAttributeID
    int mQueueIndex;
    int mQueueCount;
    ShuffleMode mShuffleMode;
    RepeatMode mRepeatMode;
    // TrackAttributeID
    std::string mArtist;
    std::string mAlbum;
    std::string mTitle;
    float mDuration;

    void dump() const;
  };

  using NotificationCb = std:: function < void() > ;

  enum class NotificationLevel {
    All, // get notified anytime anything changes. Probably too noisey.
    TrackTitleOnly, // only get notified when the track title is set.
  };
  // overwrites existing notification if set.
  void RegisterForNotifications(const NotificationCb & callback, NotificationLevel level);
  const MediaInformation & GetMediaInformation();
  void setRemoteCommandValue(uint8_t commandID);
  bool StartMediaService(BLEClient * client);
}