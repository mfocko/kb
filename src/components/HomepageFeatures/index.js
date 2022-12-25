import React from "react";
import clsx from "clsx";
import styles from "./styles.module.css";

const FeatureList = [
  {
    title: "About Me",
    description: (
      <p>
        I'm working in Red Hat in the <a href="https://github.com/packit">Packit
          team</a> and studying at <a href="https://fi.muni.cz">FI MUNI</a> while
        also tutoring some courses there.
      </p>
    ),
  },
  {
    title: "Content",
    description: (
      <>
        On this page you can find my blog or unofficial materials I have written
        over the course of teaching multiple courses at the FI.
      </>
    ),
  },
  {
    title: "This web",
    description: (
      <>
        This content can be found as a source at{" "}
        <a href="https://gitlab.com/mfocko/kb">GitLab</a> or{" "}
        <a href="https://gitlab.fi.muni.cz/xfocko/kb">FI GitLab</a>. And you can
        find it deployed on {" "}
        <a href="https://fi.muni.cz/~xfocko/kb">aisa</a> or{" "}
        <a href="https://blog.mfocko.xyz">poincare</a>.
      </>
    ),
  },
];

function Feature({ title, description }) {
  return (
    <div className={clsx("col col--4")}>
      <div className="text--center padding-horiz--md">
        <h3>{title}</h3>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures() {
  return (
    <section className={styles.features}>
      <div className="container">
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}
