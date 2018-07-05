type file = {id: string};

type me = {
  firstName: string,
  answeringMessageFile: option(file),
};

module Home = [%graphql
  {|
    query home {
      me @bsRecord {
        firstName
        answeringMessageFile @bsRecord {
          id
        }
      }
    }
  |}
];

module HomeQuery = ReasonApollo.CreateQuery(Home);

module HomeData = {
  let component = ReasonReact.statelessComponent("HomeData");

  let make = (~me: me, _children) => {
    ...component,
    render: _self => {
      let {firstName} = me;
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
                  {js|Soit pas timide, et enregistre ton message de répondeur.|js},
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
          <AudioRecorder />
        </div>
      </section>;
    },
  };
};

let component = ReasonReact.statelessComponent("HomePage");

let make = _children => {
  ...component,
  render: _self =>
    <HomeQuery>
      ...(
           ({result}) =>
             switch (result) {
             | Loading => <LoadingSection />
             | Error(error) => <ErrorSection errorMessage=error##message />
             | Data(data) => <HomeData me=data##me />
             }
         )
    </HomeQuery>,
};
