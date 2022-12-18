// @ts-check
// Note: type annotations allow type checking and IDEs autocompletion

const lightCodeTheme = require("./src/prism-themes/solarizedLight");
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
        editUrl: "https://gitlab.com/mfocko/kb/tree/main",
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

  footer() {
    return {
      label: `${this.subject.toUpperCase()}: ${this.description}`,
      to: this.subject,
    }
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
    locales: ["en"],
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

  plugins: [
    ...subjects.map(s => s.docs()),
    [
      '@docusaurus/plugin-content-blog',
      {
        id: 'blog',
        routeBasePath: 'blog',
        path: './blog',
        feedOptions: {
          type: 'all',
          copyright: `Copyright © ${new Date().getFullYear()} Matej Focko.`,
        },
        editUrl: "https://gitlab.com/mfocko/kb/tree/main",
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
          ...subjects.map(s => s.navbar()),
          {
            to: "blog",
            position: "right",
            label: "Blog",
          },
        ],
      },
      footer: {
        style: "dark",
        copyright: `Copyright © ${new Date().getFullYear()} Matej Focko.`,
        links: [
          {
            title: "Additional materials for φ courses",
            items: subjects.map(s => s.footer()),
          },
          {
            title: "Social #1",
            items: [
              {
                label: "LinkedIn",
                href: "https://www.linkedin.com/in/mfocko/",
              },
              {
                label: "GitHub",
                href: "https://github.com/mfocko",
              },
              {
                label: "GitLab",
                href: "https://gitlab.com/mfocko",
              },
            ],
          },
          {
            title: "Social #2",
            items: [
              {
                label: "Twitter",
                href: "https://twitter.com/m4tt_314",
              },
              {
                label: "Twitch",
                href: "https://twitch.tv/m4tt_314",
              },
              {
                label: "Ko-fi",
                href: "https://ko-fi.com/m4tt_314",
              }
            ]
          },
          {
            title: "Source of this web",
            items: [
              {
                label: "GitLab",
                href: "https://gitlab.com/mfocko/kb",
              },
              {
                label: "GitLab FI",
                href: "https://gitlab.fi.muni.cz/xfocko/kb",
              },
            ]
          }
        ],
      },
      prism: {
        theme: lightCodeTheme,
        darkTheme: darkCodeTheme,
        additionalLanguages: [
          "ada",
          "bash",
          "csharp",
          "dot",
          "haskell",
          "java",
          "nix",
          "pascal",
          "ruby",
          "rust",
        ],
      },
      docs: {
        sidebar: {
          hideable: true,
        }
      }
    }),
};

module.exports = config;
