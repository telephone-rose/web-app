type t;
type arrayBuffer;

[@bs.deriving abstract]
type makeBlobOptions = {
  [@bs.as "type"]
  type_: string,
};

[@bs.new]
external _makeBlob: (array(arrayBuffer), makeBlobOptions) => Fetch.blob =
  "Blob";

[@bs.get] external getBlobSize: Fetch.blob => int = "size";
[@bs.get] external getBlobType: Fetch.blob => string = "type";

[@bs.module "file-loader!opus-recorder/dist/encoderWorker.min.js"]
external _encoderWorker: string = "default";

[@bs.deriving abstract]
type makeOptions = {encoderPath: string};

[@bs.module "opus-recorder"]
external isRecordingSupported: unit => bool = "isRecordingSupported";
[@bs.new] [@bs.module] external _make: makeOptions => t = "opus-recorder";

let make = () => _make(makeOptions(~encoderPath=_encoderWorker));

[@bs.send] external start: t => unit = "";
[@bs.send] external stop: t => unit = "";
[@bs.send] external resume: t => unit = "";
[@bs.send] external pause: t => unit = "";
[@bs.send] external clearStream: t => unit = "";
[@bs.set]
external _ondataavailable: (t, arrayBuffer => unit) => unit =
  "ondataavailable";
[@bs.set] external onpause: (t, unit => unit) => unit = "";
[@bs.set] external onresume: (t, unit => unit) => unit = "";
[@bs.set] external onstart: (t, unit => unit) => unit = "";
[@bs.set] external onstop: (t, unit => unit) => unit = "";

let ondataavailable = (recorder, callback) =>
  recorder
  ->_ondataavailable(data =>
      callback(_makeBlob([|data|], makeBlobOptions(~type_="audio/opus")))
    );
