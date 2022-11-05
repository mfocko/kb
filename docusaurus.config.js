// @ts-check
// Note: type annotations allow type checking and IDEs autocompletion

const lightCodeTheme = require("prism-react-renderer/themes/vsLight");
const darkCodeTheme = require("prism-react-renderer/themes/dracula");

const math = require("remark-math");
const katex = require("rehype-katex");

require("dotenv").config();

class Subject {
  constructor(subject, description) {
    this.subject = subject;
    this.description = description;
  }

  docs() {
    return [
      "@docusaurus/plugin-content-docs",
      {
        id: this.subject,
        path: this.subject,
        routeBasePath: this.subject,
        sidebarPath: require.resolve("./sidebars.js"),
        editUrl: "https://gitlab.fi.muni.cz/xfocko/kb/tree/main",
        remarkPlugins: [math],
        rehypePlugins: [katex],
      },
    ];
  }

  navbar() {
    return {
      type: "doc",
      docId: `${this.subject}-intro`,
      docsPluginId: this.subject,
      position: "left",
      label: `${this.subject.toUpperCase()}: ${this.description}`,
    };
  }
}

const subjects = [
    new Subject("ib002", "Algorithms"),
    // new Subject("ib015", "Non-imperative programming"),
    // new Subject("ib110", "Introduction to informatics"),
    // new Subject("ib111", "Foundations of programming"),
    new Subject("pb071", "C"),
    new Subject("pb161", "C++"),
];

/** @type {import('@docusaurus/types').Config} */
const config = {
  title: "Additional materials by mf",
  tagline: "Additional materials",
  url: process.env.URL,
  baseUrl: process.env.BASE_URL,
  onBrokenLinks: "warn",
  onBrokenMarkdownLinks: "warn",
  favicon: "img/favicon.ico",

  i18n: {
    defaultLocale: "en",
    locales: ["en", "cs"],
  },

  presets: [
    [
      "classic",
      /** @type {import('@docusaurus/preset-classic').Options} */
      ({
        docs: false,
        blog: false,
        theme: {
          customCss: require.resolve("./src/css/custom.css"),
        },
      }),
    ],
  ],

  plugins: subjects.map(s => s.docs()),

  stylesheets: [
    {
      href: "https://cdn.jsdelivr.net/npm/katex@0.13.24/dist/katex.min.css",
      type: "text/css",
      integrity:
        "sha384-odtC+0UGzzFL/6PNoE8rX/SPcQDXBJ+uRepguP4QkPCm2LBxH3FA3y+fKSiJ+AmM",
      crossorigin: "anonymous",
    },
  ],

  themeConfig:
    /** @type {import('@docusaurus/preset-classic').ThemeConfig} */
    ({
      navbar: {
        title: "Additional materials by mf",
        items: [
          ...subjects.map(s => s.navbar()),
          {
            href: "https://gitlab.fi.muni.cz/xfocko/fi",
            label: "GitLab",
            position: "right",
          },
        ],
      },
      footer: {
        style: "dark",
        copyright: `Copyright © ${new Date().getFullYear()} Matej Focko.`,
      },
      prism: {
        theme: lightCodeTheme,
        darkTheme: darkCodeTheme,
        additionalLanguages: [
          "csharp",
          "java",
          "dot",
          "nix",
          "haskell",
          "rust",
        ],
      },
    }),
};

module.exports = config;
