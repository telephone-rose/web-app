type recordingState =
  | Recording
  | Stoped
  | Loading;

type state = {
  recorder: option(OpusRecorder.t),
  recordingState,
};

type action =
  | RecorderCreated(OpusRecorder.t)
  | StartRecording
  | RecordingStarted
  | RecordingStoped
  | StopRecording;

let component = ReasonReact.reducerComponent("AudioRecorder");

let make = (_children, ~onRecordReady) => {
  ...component,
  initialState: () => {recorder: None, recordingState: Loading},
  reducer: (action, state) =>
    switch (action) {
    | RecordingStarted =>
      ReasonReact.Update({...state, recordingState: Recording})
    | RecordingStoped =>
      ReasonReact.Update({...state, recordingState: Stoped})
    | StartRecording =>
      ReasonReact.UpdateWithSideEffects(
        {...state, recordingState: Loading},
        (
          self =>
            switch (self.state.recorder) {
            | None => ()
            | Some(recorder) => recorder |> OpusRecorder.start
            }
        ),
      )
    | StopRecording =>
      ReasonReact.UpdateWithSideEffects(
        {...state, recordingState: Loading},
        (
          self =>
            switch (self.state.recorder) {
            | None => ()
            | Some(recorder) => recorder |> OpusRecorder.stop
            }
        ),
      )
    | RecorderCreated(recorder) =>
      ReasonReact.Update({recordingState: Stoped, recorder: Some(recorder)})
    },
  didMount: self =>
    if (OpusRecorder.isRecordingSupported()) {
      let recorder = OpusRecorder.make();
      recorder |. OpusRecorder.onstart(() => self.send(RecordingStarted));
      recorder |. OpusRecorder.onstop(() => self.send(RecordingStoped));
      recorder |. OpusRecorder.ondataavailable(data => onRecordReady(data));
      self.onUnmount(() => recorder |> OpusRecorder.clearStream);
      self.send(RecorderCreated(recorder));
    },
  render: self =>
    switch (self.state.recorder) {
    | None => <div> (ReasonReact.string("Not supported")) </div>
    | Some(_) =>
      switch (self.state.recordingState) {
      | Loading =>
        <button
          className="button is-loading is-fullwidth is-medium"
          key="stop-recording">
          <span className="icon is-small">
            <i className="fas fa-microphone" />
          </span>
          <span> (ReasonReact.string("Rec.")) </span>
        </button>
      | Stoped =>
        <button
          className="button is-fullwidth is-medium"
          key="stop-recording"
          onClick=(_ => self.send(StartRecording))>
          <span className="icon is-small">
            <i className="fas fa-microphone" />
          </span>
          <span> (ReasonReact.string("Rec.")) </span>
        </button>
      | Recording =>
        <button
          className="button is-fullwidth is-medium"
          key="stop-recording"
          onClick=(_ => self.send(StopRecording))>
          <span className="icon is-small has-text-danger">
            <i className="fas fa-microphone" />
          </span>
          <span> (ReasonReact.string("Rec.")) </span>
        </button>
      }
    },
};
