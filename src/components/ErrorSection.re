let component = ReasonReact.statelessComponent("ErrorSection");

let make = (~errorMessage=?, _children) => {
  ...component,
  render: _self =>
    <section className="section">
      <div className="container">
        <h1 className="title"> (ReasonReact.string({js|Oups 😬|js})) </h1>
        <h2 className="subtitle">
          (ReasonReact.string({js|C'était pas censé arriver...|js}))
        </h2>
        <div className="content">
          (
            switch (errorMessage) {
            | None =>
              <p>
                (
                  ReasonReact.string(
                    {js|En plus le bug est vraiment chelou.|js},
                  )
                )
              </p>
            | Some(errorMessage) =>
              <Fragment>
                <p> (ReasonReact.string({j|Et il s'est passé ça.|j})) </p>
                <pre> (ReasonReact.string(errorMessage)) </pre>
              </Fragment>
            }
          )
        </div>
      </div>
    </section>,
};
