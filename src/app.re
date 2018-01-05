open Json.Decode;
open Types;

[%bs.raw {|require('./app.css')|}];

let styles = Css.({
  "container": style([
    display(Flex),
    flexWrap(Wrap)
  ])
});

type action = 
  | GetRepos(repositories)
  | GetRepoIssues(issues)
  | GetRepoLabels(issueLabels)
  | FilterRepoIssues(string);

let filterRepoIssues = label => FilterRepoIssues(label);

type state = {
  repositories: option(repositories),
  issues: option(issues),
  selectedIssues: option(issues),
  labels: issueLabels
};

let parseIssueLabelsResponseJson = (json: Js.Json.t) :issueLabel => {
  id: json |> field("id", int),
  name: json |> field("name", string),
  color: json |> field("color", string)
};
let parseIssuesResponseJson = (json: Js.Json.t) :issue => 
{
  id: json |> field("id", int),
  title: json |> field("title", string),
  body: json |> field("body", string),
  state: json |> field("state", string),
  html_url: json |> field("html_url", string),
  labels: json |> field("labels", array(parseIssueLabelsResponseJson))
};
let parseIssuesArrayResponseJson = (json: Js.Json.t) :issues => array(parseIssuesResponseJson, json);

let parseLabelsArrayResponseJson = (json: Js.Json.t) :issueLabels => array(parseIssueLabelsResponseJson, json);

let fetchIssues = (issuesUrl) => {
  Services.getIssuesForRepo(issuesUrl)
  |> Js.Promise.then_(Bs_fetch.Response.text)
  |> Js.Promise.then_(
    fun(jsonText) => {
      Js.Promise.resolve(parseIssuesArrayResponseJson(Js.Json.parseExn(jsonText)))
    }
  );
};

let fetchLabels = (labelsUrl) => {
  Services.getLabelsForRepo(labelsUrl)
  |> Js.Promise.then_(Bs_fetch.Response.text)
  |> Js.Promise.then_(
    fun(jsonText) => {
      Js.Promise.resolve(parseLabelsArrayResponseJson(Js.Json.parseExn(jsonText)))
    }
  );
};

let getRepoIssues = (repos, self) => {
  let handleIssuesLoaded = self.ReasonReact.reduce(issues => GetRepoIssues(issues));
  let handleLabelsLoaded = self.ReasonReact.reduce(labels => GetRepoLabels(labels));
  Array.iter((repo: repository) => {
    let url: string = repo.url;
    fetchIssues(url)
    |> Js.Promise.then_ (fun(issues: issues) => {
      handleIssuesLoaded(issues);
      Js.Promise.resolve ();
    });
    fetchLabels(url)
    |> Js.Promise.then_ (fun(labels: issueLabels) => {
      handleLabelsLoaded(labels);
      Js.Promise.resolve ();
    });
    Js.log(url);
  }, repos);
};

let component = ReasonReact.reducerComponent("App");

let make = (_children) => {
  let parseResponseJson = (json: Js.Json.t) :repository => 
      {
        id: json |> field("id", int),
        name: json |> field("name", string),
        full_name: json |> field("full_name", string),
        html_url: json |> field("html_url", string),
        open_issues_count: json |> field("open_issues_count", int),
        issues_url: json |> field("issues_url", string),
        labels_url: json |> field("labels_url", string),
        url: json |> field("url", string)
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
    initialState: fun() => {repositories: None, issues: None, selectedIssues: None, labels: [||]},
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
      | GetRepoIssues (issues) => {
          let oldIssues = switch(state.issues) {
          | Some(issues) => issues
            |> Array.to_list
            |> List.filter(issue => issue.state === "open")
            |> Array.of_list;
          | None => [||];
          };
          let newIssues = Array.append(issues, oldIssues);
          ReasonReact.Update({...state, issues: Some(newIssues)})
        }
      | GetRepoLabels (labels) => {
          let oldLabels = state.labels;
          let oldLabelsList = Array.to_list(oldLabels);
          let labelsList = Array.to_list(labels);
          let rec find = (e) =>
            fun
            | [] => false
            | [h, ...t] => h == e || find(e, t);

          let rec help_append_list = (l1, l2) =>
            switch l1 {
            | [] => l2
            | [h, ...t] =>
              if (find(h, l2)) {
                help_append_list(t, l2);
              } else {
                help_append_list(t, [h, ...l2]);
              }
            };
          ReasonReact.Update({...state, labels: Array.of_list(help_append_list(oldLabelsList, labelsList))})
        }
      | FilterRepoIssues (label) => {
          let issues = switch(state.issues) {
            | Some(issues) => issues
              |> Array.to_list
              |> List.filter((issue: issue) => {
                let labels = issue.labels;
                labels
                                        |> Array.to_list
                                        |> List.filter(l => l.name == label)
                                        |> List.length > 0})
              |> Array.of_list;
            | None => [||];
            };
          ReasonReact.Update({...state, selectedIssues: Some(issues)})
        }
      },
    render: fun({state, reduce}) => {
      let repoItems = switch (state.repositories) {
        | Some(repos) => ReasonReact.arrayToElement (Array.map(
            (fun (repo: repository) => <ListItem key=repo.name name=repo.name url=repo.html_url issues=repo.open_issues_count />), repos)
          )
        | None => ReasonReact.stringToElement("Loading Repos...")
      };
      let selectedIssues = switch(state.selectedIssues){
      | Some(issues) => ReasonReact.arrayToElement (
              issues
              |> Array.map(issue => <ListItem key=string_of_int(issue.id) name=issue.title url=issue.html_url />)
          )
      | None => ReasonReact.stringToElement("Loading Issues...")
      };
      <div className="App">
        <h2> (ReasonReact.stringToElement("Issues from Starred Repos")) </h2>
        <Filter labels=state.labels onFilterChange=(reduce(filterRepoIssues))/>
        <div className=styles##container>
          /* <LoginButton /> */
          <Liste>
            {selectedIssues}
          </Liste>
          <Liste>
            {repoItems}
          </Liste>
        </div>
      </div>
    }
  }
};
