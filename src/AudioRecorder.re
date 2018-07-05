type state = {
  isRecording: bool,
  record: option(Navigator.objectUrl),
  recorder: option((unit => unit, unit => unit, unit => unit)),
};

type action =
  | Record
  | StopRecording
  | RecorderAvailable((unit => unit, unit => unit, unit => unit))
  | RecordAvailable(Navigator.objectUrl);

let component = ReasonReact.reducerComponent("MessageRecorder");

let make = _children => {
  ...component,
  initialState: () => {isRecording: false, recorder: None, record: None},
  reducer: (action, state) =>
    switch (action) {
    | StopRecording =>
      ReasonReact.UpdateWithSideEffects(
        {...state, isRecording: false},
        (
          self =>
            switch (self.state.recorder) {
            | None => ()
            | Some((_, stop, _)) => stop()
            }
        ),
      )

    | Record =>
      ReasonReact.UpdateWithSideEffects(
        {...state, isRecording: true},
        (
          self =>
            switch (self.state.recorder) {
            | None => ()
            | Some((play, _, _)) => play()
            }
        ),
      )
    | RecordAvailable(record) =>
      ReasonReact.Update({...state, record: Some(record)})
    | RecorderAvailable(controls) =>
      ReasonReact.Update({...state, recorder: Some(controls)})
    },
  didMount: self =>
    Navigator.recordAudio(
      controls => self.send(RecorderAvailable(controls)),
      record => self.send(RecordAvailable(record)),
    ),
  render: self =>
    <div className="field has-addons">
      <p className="control">
        <button
          className="button"
          onClick=(_ => self.send(Record))
          disabled=(self.state.recorder === None ? true : false)>
          (
            self.state.isRecording ?
              <span className="icon is-small has-text-danger">
                <i className="fas fa-square" />
              </span> :
              <span className="icon is-small has-text-danger">
                <i className="fas fa-circle" />
              </span>
          )
          <span> (ReasonReact.string("Rec.")) </span>
        </button>
      </p>
      <p className="control">
        <button
          className="button"
          disabled=(self.state.record === None ? true : false)>
          <span className="icon is-small">
            <i className="fas fa-play" />
          </span>
          <span> (ReasonReact.string("Play")) </span>
        </button>
      </p>
    </div>,
};
