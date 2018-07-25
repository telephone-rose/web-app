/* navigator.mediaDevices.getUserMedia({ audio: true })
   .then(stream => {
     const mediaRecorder = new MediaRecorder(stream);
     mediaRecorder.start();
   }); */

type navigator;
type mediaDevices;
type stream;
type mediaRecorder;
type audioChunk;
type blob;
type url;
type objectUrl;
type audio;

[@bs.deriving abstract]
type getUserMediaOptions = {audio: option(bool)};

[@bs.deriving abstract]
type recordingDataEvent = {data: audioChunk};

[@bs.val] [@bs.return nullable]
external _navigator : option(navigator) = "navigator";

[@bs.get]
external _getMediaDevices : navigator => mediaDevices = "mediaDevices";

[@bs.send]
external _getUserMedia :
  (mediaDevices, getUserMediaOptions) => Js.Promise.t(stream) =
  "getUserMedia";

[@bs.new] external _mediaRecorder : stream => mediaRecorder = "MediaRecorder";

[@bs.send] external _startRecording : mediaRecorder => unit = "start";

[@bs.send] external _stopRecording : mediaRecorder => unit = "stop";

[@bs.send]
external _addEventListener :
  (
    mediaRecorder,
    [@bs.string] [
      | `dataavailable(recordingDataEvent => unit)
      | `stop(unit => unit)
    ]
  ) =>
  unit =
  "addEventListener";

[@bs.new] external _makeBlob : array(audioChunk) => blob = "Blob";

[@bs.val] [@bs.return nullable] external _url : option(url) = "URL";

[@bs.send]
external _createObjectURL : (url, blob) => objectUrl = "createObjectURL";

[@bs.val] [@bs.return nullable] external audio : option(audio) = "Audio";

[@bs.send] external play : (audio, objectUrl) => unit = "play";

let recordAudio = (controllsCallback, onStop) =>
  (
    switch (_navigator, _url) {
    | (Some(navigator), Some(url)) =>
      let mediaDevices = navigator |> _getMediaDevices;
      _getUserMedia(mediaDevices, getUserMediaOptions(~audio=Some(true)))
      |> Js.Promise.then_(stream => {
           let mediaRecorder = stream |> _mediaRecorder;
           let audioChunks: ref(list(audioChunk)) = ref([]);

           mediaRecorder
           |. _addEventListener(
                `dataavailable(
                  (rde: recordingDataEvent) => {
                    audioChunks := List.append(audioChunks^, [data(rde)]);
                    ();
                  },
                ),
              )
           |> ignore;

           mediaRecorder
           |. _addEventListener(
                `stop(
                  () => {
                    /* let const audioBlob = new Blob(audioChunks); */
                    let audioBlob = audioChunks^ |> Array.of_list |> _makeBlob;
                    let audioUrl = _createObjectURL(url, audioBlob);
                    onStop(audioUrl);
                    ();
                  },
                ),
              )
           |> ignore;

           controllsCallback((
             () => mediaRecorder |> _startRecording,
             () => mediaRecorder |> _stopRecording,
             () => audioChunks := [],
           ));
           Js.Promise.resolve();
         });
    | _ => Js.Promise.resolve()
    }
  )
  |> ignore;
