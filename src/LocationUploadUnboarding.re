type action =
  | ShowLoading;
type state = {isLoading: bool};

let component = ReasonReact.reducerComponent("LocationUploadUnboarding");

let make = (~onSkip, _children) => {
  ...component,
  initialState: () => {isLoading: false},
  reducer: (action, _state) =>
    switch (action) {
    | ShowLoading => ReasonReact.Update({isLoading: true})
    },
  render: self =>
    <section className="section">
      <div className="container">
        <h1 className="title">
          (ReasonReact.string({j|Et en fait, tu viens d'ou ?|j}))
        </h1>
        <h2 className="subtitle">
          (
            ReasonReact.string(
              {js|Tokyo ? Berlin ? Châlons-en-Champagne ?|js},
            )
          )
        </h2>
        <div className="content">
          <p>
            (
              ReasonReact.string(
                {js|Promis, pas de mauvaise blague, c'est simplement pour te faire matcher avec des gens au plus proche de chez toi|js},
              )
            )
          </p>
          <p>
            (ReasonReact.string({js|Comme d'hab, tu pourra a |js}))
            <strong>
              (ReasonReact.string({js|nimporte quel moment|js}))
            </strong>
            (
              ReasonReact.string(
                {js| désactiver la localisation depuis les réglages, |js},
              )
            )
            <strong> (ReasonReact.string({js| pas de stress|js})) </strong>
            (ReasonReact.string({js| 😌.|js}))
          </p>
        </div>
        <UpdateLocation.Mutation>
          ...(
               (mutate, _) =>
                 <button
                   onClick=(
                     _ => {
                       self.send(ShowLoading);
                       Geolocation.getCurrentPosition(position => {
                         let variables =
                           UpdateLocation.Payload.make(
                             ~location={
                               "latitude": position.latitude,
                               "longitude": position.longitude,
                             },
                             (),
                           );
                         Js.Promise.(
                           mutate(~variables=variables##variables, ())
                           |> then_(response =>
                                resolve(
                                  UpdateLocation.Mutation.convertJsInputToReason(
                                    response,
                                  ),
                                )
                              )
                           |> then_(
                                (
                                  response: UpdateLocation.Mutation.renderPropObj,
                                ) => {
                                switch (response.data) {
                                | Some(_) =>
                                  onSkip();
                                  ();
                                | _ => ()
                                };
                                resolve();
                              })
                           |> ignore
                         );

                         ();
                       });
                     }
                   )
                   className=(
                     "button is-primary is-medium is-fullwidth "
                     ++ Theme.marginTop
                     ++ (self.state.isLoading ? " is-loading" : "")
                   )>
                   (ReasonReact.string({js|Partager ma position|js}))
                 </button>
             )
        </UpdateLocation.Mutation>
      </div>
    </section>,
};
