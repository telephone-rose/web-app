module UpdateAnsweringMessage = {
  type updateAnsweringMessage = {id: string};

  module UpdateAnsweringMessage = [%graphql
    {|
    mutation UpdateAnsweringMessage (
      $recordingId: ID!
    ) {
      updateAnsweringMessage (
        recordingId: $recordingId
      ) @bsRecord {
        id
      }
    }
  |}
  ];

  module Mutation = ReasonApollo.CreateMutation(UpdateAnsweringMessage);
};

type action =
  | SetRecording(RecordCreator.recording)
  | ShowError;
type state = {
  recording: option(RecordCreator.recording),
  hasError: bool,
};

let component = ReasonReact.reducerComponent("RecordingUploadUnboarding");

let make = (~firstName: string, ~onSkip, _children) => {
  ...component,
  initialState: () => {recording: None, hasError: false},
  reducer: (action, state) =>
    switch (action) {
    | SetRecording(recording) =>
      ReasonReact.Update({hasError: false, recording: Some(recording)})
    | ShowError => ReasonReact.Update({...state, hasError: true})
    },
  render: self =>
    <section className="section">
      <div className="container">
        <h1 className="title">
          (ReasonReact.string({j|Salut $firstName 🤩|j}))
        </h1>
        <h2 className="subtitle">
          (ReasonReact.string({js|On dirait bien que t'es nouveau ici.|js}))
        </h2>
        <div className="content">
          <p>
            (
              ReasonReact.string(
                {js|Pour commencer, tu peux déja te présenter !|js},
              )
            )
          </p>
          <p>
            (
              ReasonReact.string(
                {js|Sois pas timide, et enregistre ton message de répondeur.|js},
              )
            )
          </p>
          <p>
            (
              ReasonReact.string(
                {js|Respire un bon coup et lance toi ! Au pire, tu pourra ensuite le modifier |js},
              )
            )
            <strong> (ReasonReact.string({js|quand tu veux|js})) </strong>
            (ReasonReact.string({js| et |js}))
            <strong> (ReasonReact.string({js|autant de fois|js})) </strong>
            (ReasonReact.string({js| que tu veux.|js}))
          </p>
        </div>
        <div>
          (
            if (self.state.hasError) {
              <div
                className=("has-text-centered subtitle " ++ Theme.marginBottom)>
                <strong className="has-text-danger">
                  (
                    ReasonReact.string(
                      {js|PARLE PLUS FORT, J'ENTENDS RIEN 👴|js},
                    )
                  )
                </strong>
              </div>;
            } else {
              ReasonReact.null;
            }
          )
        </div>
        <RecordCreator
          onRecordingCreation=(
            recording => self.send(SetRecording(recording))
          )
          onRecordingError=(() => self.send(ShowError))
        />
        (
          switch (self.state.recording) {
          | Some(recording) =>
            <UpdateAnsweringMessage.Mutation>
              ...(
                   (mutate, _) =>
                     <button
                       className=(
                         "button is-primary is-medium is-fullwidth "
                         ++ Theme.marginTop
                       )
                       onClick=(
                         _ => {
                           let variables =
                             UpdateAnsweringMessage.UpdateAnsweringMessage.make(
                               ~recordingId=recording.id,
                               (),
                             );
                           Js.Promise.(
                             mutate(~variables=variables##variables, ())
                             |> then_(response =>
                                  resolve(
                                    UpdateAnsweringMessage.Mutation.convertJsInputToReason(
                                      response,
                                    ),
                                  )
                                )
                             |> then_(
                                  (
                                    response: UpdateAnsweringMessage.Mutation.renderPropObj,
                                  ) => {
                                  switch (response.data) {
                                  | Some(_) =>
                                    onSkip();
                                    ();
                                  | _ => self.send(ShowError)
                                  };
                                  resolve();
                                })
                             |> ignore
                           );
                         }
                       )>
                       (
                         ReasonReact.string(
                           {js|👍 On passe à la suite ?|js},
                         )
                       )
                     </button>
                 )
            </UpdateAnsweringMessage.Mutation>
          | None =>
            <button
              className=(
                "button is-light is-small is-fullwidth " ++ Theme.marginTop
              )
              onClick=(_ => onSkip())>
              (ReasonReact.string({js|Ou juste faire ça plus tard|js}))
            </button>
          }
        )
      </div>
    </section>,
};
