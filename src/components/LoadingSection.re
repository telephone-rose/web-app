let component = ReasonReact.statelessComponent("LoadingSection");

let make = _children => {
  ...component,
  render: _self =>
    <section className="section">
      <div className="container">
        <h1 className="title">
          (ReasonReact.string({js|Ça charge...|js}))
        </h1>
        <h2 className="subtitle">
          (
            ReasonReact.string({js|Et ça devrait plus être très long...|js})
          )
        </h2>
      </div>
    </section>,
};
