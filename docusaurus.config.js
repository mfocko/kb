// @ts-check
// Note: type annotations allow type checking and IDEs autocompletion

const lightCodeTheme = require("prism-react-renderer/themes/vsLight");
const darkCodeTheme = require("prism-react-renderer/themes/dracula");

const math = require("remark-math");
const katex = require("rehype-katex");

require("dotenv").config();

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
        docs: {
          path: "ib002",
          routeBasePath: "ib002",
          sidebarPath: require.resolve("./sidebars.js"),
          editUrl: "https://gitlab.fi.muni.cz/xfocko/kb/tree/main",
          remarkPlugins: [math],
          rehypePlugins: [katex],
        },
        blog: false,
        theme: {
          customCss: require.resolve("./src/css/custom.css"),
        },
      }),
    ],
  ],

  plugins: [
    [
      "@docusaurus/plugin-content-docs",
      {
        id: "ib015",
        path: "ib015",
        routeBasePath: "ib015",
        sidebarPath: require.resolve("./sidebars.js"),
        editUrl: "https://gitlab.fi.muni.cz/xfocko/kb/tree/main",
        remarkPlugins: [math],
        rehypePlugins: [katex],
      },
    ],
    [
      "@docusaurus/plugin-content-docs",
      {
        id: "ib110",
        path: "ib110",
        routeBasePath: "ib110",
        sidebarPath: require.resolve("./sidebars.js"),
        editUrl: "https://gitlab.fi.muni.cz/xfocko/kb/tree/main",
        remarkPlugins: [math],
        rehypePlugins: [katex],
      },
    ],
    [
      "@docusaurus/plugin-content-docs",
      {
        id: "pb071",
        path: "pb071",
        routeBasePath: "pb071",
        sidebarPath: require.resolve("./sidebars.js"),
        editUrl: "https://gitlab.fi.muni.cz/xfocko/kb/tree/main",
        remarkPlugins: [math],
        rehypePlugins: [katex],
      },
    ],
    [
      "@docusaurus/plugin-content-docs",
      {
        id: "pb161",
        path: "pb161",
        routeBasePath: "pb161",
        sidebarPath: require.resolve("./sidebars.js"),
        editUrl: "https://gitlab.fi.muni.cz/xfocko/kb/tree/main",
        remarkPlugins: [math],
        rehypePlugins: [katex],
      },
    ],
  ],

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
