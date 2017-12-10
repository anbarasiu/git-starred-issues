open Json.Decode;

[%bs.raw {|require('./app.css')|}];

type repository = {
  id: int,
  name: string,
  full_name: string,
  html_url: string,
  open_issues_count: int
};

type repositories = array(repository);

type issue = {
  id: int
};

type issues = array(issue);

type action = 
  | GetRepos(repositories)
  | GetRepoIssues(issues);

type state = {
  repositories: option(repositories),
  issues: option(issues)
};

let getRepoIssues = (repos, self) => {
  Array.iter((repo: repository) => {
    let repoId: int = repo.id;
    Js.log(repoId);
    /* self.reduce(repoId => GetRepoIssues(repoId)) */
  }, repos);
  Js.log(repos);
};

let component = ReasonReact.reducerComponent("App");

let make = (_children) => {
  let parseResponseJson = (json: Js.Json.t) :repository => 
      {
        id: json |> field("id", int),
        name: json |> field("name", string),
        full_name: json |> field("full_name", string),
        html_url: json |> field("html_url", string),
        open_issues_count: json |> field("open_issues_count", int)
      };
  let parseArrayResponseJson = (json: Js.Json.t) :repositories => array(parseResponseJson, json);
  let fetchRepos = () => {
    Services.getStarredRepos()
    |> Js.Promise.then_(Bs_fetch.Response.text)
    |> Js.Promise.then_(
      fun(jsonText) => {
        Js.Promise.resolve(parseArrayResponseJson(Js.Json.parseExn(jsonText)))
      }
    );
  };
  {
    ...component,
    initialState: fun() => {repositories: None, issues: None},
    didMount: (self) => {
      let handleReposLoaded = self.reduce(repositories => GetRepos(repositories));
      fetchRepos()
      |> Js.Promise.then_ (fun(repos) => {
        handleReposLoaded(repos);
        Js.Promise.resolve ();
      });
      ReasonReact.NoUpdate
    },
    reducer: fun(action, state) =>
      switch action {
      | GetRepos (repos) => ReasonReact.UpdateWithSideEffects({...state, repositories: Some(repos)}, (self) => getRepoIssues(repos, self))
      | GetRepoIssues (issues) => ReasonReact.Update {...state, issues: Some(issues)}
    },
    render: fun({state}) => {
      let repoItems = switch (state.repositories) {
        | Some(repos) => ReasonReact.arrayToElement (Array.map(
            (fun (repo: repository) => <ListItem key=repo.name name=repo.name url=repo.html_url issues=repo.open_issues_count />), repos)
          )
        | None => ReasonReact.stringToElement("Loading")
      };
      <div className="App">
        <h2> (ReasonReact.stringToElement("Issues from Starred Repos")) </h2>
        <div>
          /* <LoginButton /> */
          <List>
            {repoItems}
          </List>
        </div>
      </div>
    }
  }
};
