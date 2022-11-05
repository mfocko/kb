// @ts-check
// Note: type annotations allow type checking and IDEs autocompletion

const lightCodeTheme = require("prism-react-renderer/themes/vsLight");
const darkCodeTheme = require("prism-react-renderer/themes/dracula");

const math = require("remark-math");
const katex = require("rehype-katex");

require("dotenv").config();

function constructDocs(subject) {
  return [
    "@docusaurus/plugin-content-docs",
    {
      id: subject,
      path: subject,
      routeBasePath: subject,
      sidebarPath: require.resolve("./sidebars.js"),
      editUrl: "https://gitlab.fi.muni.cz/xfocko/kb/tree/main",
      remarkPlugins: [math],
      rehypePlugins: [katex],
    },
  ];
}

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

  plugins: ["ib002", "ib015", "ib110", "pb071", "pb161"].map(constructDocs),

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
          {
            type: "doc",
            docId: "ib002-intro",
            docsPluginId: "ib002",
            position: "left",
            label: "IB002: Algorithms",
          },
          // {
          //   type: "doc",
          //   docId: "ib015-intro",
          //   docsPluginId: "ib015",
          //   position: "left",
          //   label: "IB015: Non-imperative programming",
          // },
          // {
          //   type: "doc",
          //   docId: "ib110-intro",
          //   docsPluginId: "ib110",
          //   position: "left",
          //   label: "IB110: Introduction to informatics",
          // },
          {
            type: "doc",
            docId: "pb071-intro",
            docsPluginId: "pb071",
            position: "left",
            label: "PB071: C",
          },
          {
            type: "doc",
            docId: "pb161-intro",
            docsPluginId: "pb161",
            position: "left",
            label: "PB161: C++",
          },
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
