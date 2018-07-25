let component = ReasonReact.statelessComponent("LoginPage");

let make = (~onLogin, _children) => {
  ...component,
  render: _ =>
    <section className="section">
      <div className="container">
        <h1 className="title">
          (ReasonReact.string({js|Hello hello 👋|js}))
        </h1>
        <h2 className="subtitle">
          (ReasonReact.string({js|On se connaît ?|js}))
        </h2>
        <span className="subtitle">
          (ReasonReact.string({js|T'es plutôt du genre|js}))
        </span>
        <br />
        <FacebookLoginButton onLogin />
        <br />
        <br />
        <span className="subtitle"> (ReasonReact.string("ou")) </span>
        <br />
        <GoogleLoginButton onLogin />
      </div>
    </section>,
};
