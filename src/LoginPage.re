let component = ReasonReact.statelessComponent("LoginPage");

let make = (~onLogin, _children) => {
  ...component,
  render: _ =>
    <section className="section">
      <div className="container">
        <h1 className="title"> (ReasonReact.string("Hello !")) </h1>
        <span> (ReasonReact.string("-")) </span>
        <br />
        <br />
        <span className="subtitle">
          (ReasonReact.string({js|Plutôt|js}))
        </span>
        <br />
        <FacebookLoginButton onLogin />
        <br />
        <br />
        <span className="subtitle"> (ReasonReact.string("ou")) </span>
        <br />
        <button className="button is-medium is-light">
          <span className="icon"> <i className="fab fa-google" /> </span>
          <span> (ReasonReact.string("Google")) </span>
        </button>
      </div>
    </section>,
};
