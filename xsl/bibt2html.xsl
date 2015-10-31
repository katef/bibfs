<?xml version="1.0"?>

<xsl:stylesheet version="1.0"
	xmlns="http://www.w3.org/1999/xhtml"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:str="http://exslt.org/strings"
	xmlns:func="http://exslt.org/functions"
	xmlns:common="http://exslt.org/common"
	xmlns:b="http://xml.elide.org/bibt"

	extension-element-prefixes="str func common"

	exclude-result-prefixes="b str func common">

	<xsl:output indent="yes"/>

	<func:function name="b:assert">
		<xsl:param name="type"/>
		<xsl:param name="key"/>
		<xsl:param name="name"/>
		<xsl:param name="node"/>

		<xsl:if test="not($node)">
			<xsl:message terminate="yes">
				<xsl:text>Missing required field for @</xsl:text>
				<xsl:value-of select="$type"/>
				<xsl:text> </xsl:text>
				<xsl:value-of select="$key"/>
				<xsl:text>: </xsl:text>
				<xsl:value-of select="$name"/>
					<xsl:text>="..."&#10;</xsl:text>
			</xsl:message>
		</xsl:if>
	</func:function>

	<xsl:template match="b:texmath">
		<code class="texmath">
			<xsl:apply-templates/>
		</code>
	</xsl:template>

	<xsl:template match="b:field/b:value" mode="fallback">
		<span class="{../@name}">
			<xsl:apply-templates/>
			<xsl:text>.</xsl:text>
		</span>
	</xsl:template>

	<xsl:template name="monthyear">
		<xsl:param name="month"/>
		<xsl:param name="year"/>

		<xsl:choose>
			<xsl:when test="$month">
				<time datetime="{$year}-{$month}">
					<xsl:choose>
						<xsl:when test="$month =  1">
							<xsl:text>Jan.</xsl:text>
						</xsl:when>
						<xsl:when test="$month =  2">
							<xsl:text>Feb.</xsl:text>
						</xsl:when>
						<xsl:when test="$month =  3">
							<xsl:text>March</xsl:text>
						</xsl:when>
						<xsl:when test="$month =  4">
							<xsl:text>April</xsl:text>
						</xsl:when>
						<xsl:when test="$month =  5">
							<xsl:text>May</xsl:text>
						</xsl:when>
						<xsl:when test="$month =  6">
							<xsl:text>June</xsl:text>
						</xsl:when>
						<xsl:when test="$month =  7">
							<xsl:text>July</xsl:text>
						</xsl:when>
						<xsl:when test="$month =  8">
							<xsl:text>Aug.</xsl:text>
						</xsl:when>
						<xsl:when test="$month =  9">
							<xsl:text>Sept.</xsl:text>
						</xsl:when>
						<xsl:when test="$month = 10">
							<xsl:text>Oct.</xsl:text>
						</xsl:when>
						<xsl:when test="$month = 11">
							<xsl:text>Nov.</xsl:text>
						</xsl:when>
						<xsl:when test="$month = 12">
							<xsl:text>Dec.</xsl:text>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$month"/>
						</xsl:otherwise>
					</xsl:choose>
					<xsl:text>&#8201;</xsl:text>
					<xsl:apply-templates select="$year"/>
				</time>
			</xsl:when>
			<xsl:otherwise>
				<time datetime="{$year}">
					<xsl:apply-templates select="$year"/>
				</time>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="location">
		<xsl:param name="volume"  select="/.."/>
		<xsl:param name="number"  select="/.."/>
		<xsl:param name="pages"   select="/.."/>
		<xsl:param name="chapter" select="/.."/>

		<span class="location">
			<xsl:apply-templates select="$volume"/>
			<xsl:if test="$number">
				<xsl:text>(</xsl:text>
				<xsl:apply-templates select="$number"/>
				<xsl:text>)</xsl:text>
			</xsl:if>
			<xsl:if test="($volume or $number) and $pages">
				<xsl:text>:</xsl:text>
				<xsl:apply-templates select="$pages"/>
			</xsl:if>
			<xsl:if test="$chapter">
				<xsl:text> chapter&#xA0;</xsl:text>
				<xsl:apply-templates select="common:node-set($chapter)"/>
			</xsl:if>
		</span>
	</xsl:template>

	<xsl:template match="b:field">
		<xsl:apply-templates select="b:value"/>
	</xsl:template>

	<xsl:template match="b:field" mode="otherformats">
		<xsl:apply-templates select="b:value" mode="otherformats"/>
	</xsl:template>

	<xsl:template match="b:field/b:value" mode="comment">
		<xsl:comment>
			<xsl:text> </xsl:text>
			<xsl:value-of select="../@name"/>
			<xsl:text>: </xsl:text>
			<xsl:value-of select="."/>
			<xsl:text> </xsl:text>
		</xsl:comment>
	</xsl:template>

	<xsl:template match="b:field[@name = 'publisher']/b:value">
		<address class="publisher">
			<xsl:apply-templates/>
		</address>
	</xsl:template>

	<xsl:template match="b:field[@name = 'school']/b:value">
		<address class="school">
			<xsl:apply-templates/>
		</address>
	</xsl:template>

	<xsl:template match="b:field[@name = 'journal']/b:value">
		<cite class="journal">
			<xsl:apply-templates/>
			<xsl:text>,</xsl:text>
		</cite>
	</xsl:template>

	<xsl:template match="b:field[@name = 'title']/b:value">
		<cite class="title">
			<xsl:apply-templates/>
			<xsl:text>.</xsl:text>
		</cite>
	</xsl:template>

	<xsl:template match="b:field[@name = 'note']/b:value">
		<xsl:text> </xsl:text>
		<span class="note">
			<xsl:apply-templates/>
			<xsl:text>.</xsl:text>
		</span>
	</xsl:template>

	<xsl:template match="b:field[@name = 'issn']/b:value">
		<!-- TODO: a rel="something" -->
		<span class="issn">
			<xsl:apply-templates/>
		</span>
	</xsl:template>

	<xsl:template match="b:field[@name = 'isbn']/b:value">
		<!-- TODO: a rel="something" -->
		<span class="isbn">
			<xsl:apply-templates/>
		</span>
	</xsl:template>

	<xsl:template match="b:field[@name = 'url']/b:value">
		<a class="url" href="{.}" rel="external">
			<xsl:apply-templates/>
		</a>
	</xsl:template>

	<xsl:template match="b:field[@name = 'url']/b:value" mode="otherformats">
		<li>
			<xsl:choose>
				<!-- TODO: param for local website base -->
				<xsl:when test="starts-with(., 'http://www.tendra.org/')">
					<a href="/{substring-after(., 'http://www.tendra.org/')}">
						<xsl:text>HTML</xsl:text>
					</a>
				</xsl:when>
				<xsl:otherwise>
					<a href="{.}" rel="external">
						<xsl:apply-templates/>
					</a>
				</xsl:otherwise>
			</xsl:choose>
		</li>
	</xsl:template>

	<xsl:template match="b:field[@name = 'doi']/b:value" mode="otherformats">
		<li>
			<!-- TODO: a rel="alternate" -->
			<a class="doi" href="http://dx.doi.org/{.}" rel="external alternate">
				<xsl:apply-templates/>
			</a>
		</li>
	</xsl:template>

	<xsl:template match="b:field[@name = 'file']/b:value" mode="otherformats">
		<xsl:variable name="lower" select="'abcdefghijklmnopqrstuvwxyz'"/>
		<xsl:variable name="upper" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'"/>

		<xsl:variable name="type" select="substring-after(., '.')"/>

		<li>
			<!-- TODO: a rel="something" -->
			<!-- TODO: variable for base path -->
			<a href="{concat('/', .)}">
				<xsl:if test="$type">
					<xsl:attribute name="data-type">
						<xsl:value-of select="$type"/>
					</xsl:attribute>
				</xsl:if>

				<xsl:choose>
					<xsl:when test="$type">
						<xsl:value-of select="translate($type, $lower, $upper)"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="."/>
					</xsl:otherwise>
				</xsl:choose>
			</a>
		</li>
	</xsl:template>

	<xsl:template match="b:field[@name = 'author' or @name = 'editor']/b:value">
		<address class="{@name}">
			<xsl:for-each select="str:tokenize(., ',')">
				<xsl:sort select="position()" data-type="number" order="descending"/>
				<xsl:for-each select="str:tokenize(., ' ')">	<!-- TODO: also on nbsp -->
					<xsl:apply-templates/>
					<xsl:choose>
						<xsl:when test="string-length(.) = 1"> <!-- initial -->
							<xsl:text>.</xsl:text>
							<xsl:if test="position() != last()">
								<xsl:text>&#xA0;</xsl:text>
							</xsl:if>
						</xsl:when>
						<xsl:otherwise>
							<xsl:if test="position() != last()">
								<xsl:text> </xsl:text>
							</xsl:if>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:for-each>
				<xsl:if test="position() != last()">
					<xsl:text> </xsl:text>
				</xsl:if>
			</xsl:for-each>
		</address>

		<xsl:choose>
			<xsl:when test="position() = last()">
				<xsl:text>.</xsl:text>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>, </xsl:text>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="b:entry">
		<xsl:variable name="address"      select="b:field[@name = 'address'     ]"/>
		<xsl:variable name="author"       select="b:field[@name = 'author'      ]"/>
		<xsl:variable name="booktitle"    select="b:field[@name = 'booktitle'   ]"/>
		<xsl:variable name="chapter"      select="b:field[@name = 'chapter'     ]"/>
		<xsl:variable name="edition"      select="b:field[@name = 'edition'     ]"/>
		<xsl:variable name="editor"       select="b:field[@name = 'editor'      ]"/>
		<xsl:variable name="howpublished" select="b:field[@name = 'howpublished']"/>
		<xsl:variable name="institution"  select="b:field[@name = 'institution' ]"/>
		<xsl:variable name="isbn"         select="b:field[@name = 'isbn'        ]"/>
		<xsl:variable name="journal"      select="b:field[@name = 'journal'     ]"/>
		<xsl:variable name="month"        select="b:field[@name = 'month'       ]"/>
		<xsl:variable name="note"         select="b:field[@name = 'note'        ]"/>
		<xsl:variable name="number"       select="b:field[@name = 'number'      ]"/>
		<xsl:variable name="organization" select="b:field[@name = 'organization']"/>
		<xsl:variable name="pages"        select="b:field[@name = 'pages'       ]"/>
		<xsl:variable name="publisher"    select="b:field[@name = 'publisher'   ]"/>
		<xsl:variable name="series"       select="b:field[@name = 'series'      ]"/>
		<xsl:variable name="school"       select="b:field[@name = 'school'      ]"/>
		<xsl:variable name="title"        select="b:field[@name = 'title'       ]"/>
		<xsl:variable name="volume"       select="b:field[@name = 'volume'      ]"/>
		<xsl:variable name="year"         select="b:field[@name = 'year'        ]"/>

		<!-- non-standard -->
		<xsl:variable name="key"          select="b:field[@name = 'key'         ]"/>
		<xsl:variable name="file"         select="b:field[@name = 'file'        ]"/>
		<xsl:variable name="issn"         select="b:field[@name = 'issn'        ]"/>
		<xsl:variable name="doi"          select="b:field[@name = 'doi'         ]"/>
		<xsl:variable name="url"          select="b:field[@name = 'url'         ]"/>

		<xsl:variable name="others">
			<xsl:copy-of select="b:field[
				    @name != 'key'
				and @name != 'address'
				and @name != 'author'
				and @name != 'booktitle'
				and @name != 'chapter'
				and @name != 'edition'
				and @name != 'editor'
				and @name != 'howpublished'
				and @name != 'institution'
				and @name != 'isbn'
				and @name != 'journal'
				and @name != 'month'
				and @name != 'note'
				and @name != 'number'
				and @name != 'organization'
				and @name != 'pages'
				and @name != 'publisher'
				and @name != 'series'
				and @name != 'school'
				and @name != 'title'
				and @name != 'volume'
				and @name != 'year'
				and @name != 'file'
				and @name != 'issn'
				and @name != 'doi'
				and @name != 'url']"/>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="@type = 'article'">
				<xsl:value-of select="b:assert(@type, @key, 'author',  $author )"/>
				<xsl:value-of select="b:assert(@type, @key, 'title',   $title  )"/>
				<xsl:value-of select="b:assert(@type, @key, 'journal', $journal)"/>
				<xsl:value-of select="b:assert(@type, @key, 'year',    $year   )"/>
			</xsl:when>

			<xsl:when test="@type = 'book'">
				<xsl:value-of select="b:assert(@type, @key, 'author',    $author)"/>
				<xsl:value-of select="b:assert(@type, @key, 'title',     $title)"/>
				<xsl:value-of select="b:assert(@type, @key, 'publisher', $publisher)"/>
				<xsl:value-of select="b:assert(@type, @key, 'year',      $year  )"/>
			</xsl:when>

			<xsl:when test="@type = 'booklet'">
				<xsl:value-of select="b:assert(@type, @key, 'title',     $title)"/>
			</xsl:when>

			<xsl:when test="@type = 'conference'">
				<xsl:value-of select="b:assert(@type, @key, 'author',    $author)"/>
				<xsl:value-of select="b:assert(@type, @key, 'title',     $title)"/>
				<xsl:value-of select="b:assert(@type, @key, 'booktitle', $booktitle)"/>
				<xsl:value-of select="b:assert(@type, @key, 'year',      $year  )"/>
			</xsl:when>

			<xsl:when test="@type = 'inbook'">
				<xsl:value-of select="b:assert(@type, @key, 'author',    $author   )"/>
				<xsl:value-of select="b:assert(@type, @key, 'title',     $title    )"/>
				<xsl:value-of select="b:assert(@type, @key, 'chapter and/or pages', $chapter | $pages)"/>
				<xsl:value-of select="b:assert(@type, @key, 'publisher', $publisher)"/>
				<xsl:value-of select="b:assert(@type, @key, 'year',      $year     )"/>
			</xsl:when>

			<xsl:when test="@type = 'incollection'">
				<xsl:value-of select="b:assert(@type, @key, 'author',    $author)"/>
				<xsl:value-of select="b:assert(@type, @key, 'title',     $title)"/>
				<xsl:value-of select="b:assert(@type, @key, 'booktitle', $booktitle)"/>
				<xsl:value-of select="b:assert(@type, @key, 'year',      $year  )"/>
			</xsl:when>

			<xsl:when test="@type = 'manual'">
				<xsl:value-of select="b:assert(@type, @key, 'title',  $title)"/>
			</xsl:when>

			<xsl:when test="@type = 'mastersthesis' or @type = 'phdthesis'">
				<xsl:value-of select="b:assert(@type, @key, 'author',  $author)"/>
				<xsl:value-of select="b:assert(@type, @key, 'title',   $title )"/>
				<xsl:value-of select="b:assert(@type, @key, 'school',  $school)"/>
				<xsl:value-of select="b:assert(@type, @key, 'year',    $year  )"/>
			</xsl:when>

			<xsl:when test="@type = 'misc'">
			</xsl:when>

			<xsl:when test="@type = 'inproceedings'">
				<xsl:value-of select="b:assert(@type, @key, 'author',    $author)"/>
				<xsl:value-of select="b:assert(@type, @key, 'title',     $title)"/>
				<xsl:value-of select="b:assert(@type, @key, 'booktitle', $booktitle)"/>
				<xsl:value-of select="b:assert(@type, @key, 'year',      $year  )"/>
			</xsl:when>

			<xsl:when test="@type = 'proceedings'">
				<xsl:value-of select="b:assert(@type, @key, 'title', $title)"/>
				<xsl:value-of select="b:assert(@type, @key, 'year',  $year )"/>
			</xsl:when>

			<xsl:when test="@type = 'techreport'">
				<xsl:value-of select="b:assert(@type, @key, 'author',      $author)"/>
				<xsl:value-of select="b:assert(@type, @key, 'title',       $title )"/>
				<xsl:value-of select="b:assert(@type, @key, 'institution', $institution)"/>
				<xsl:value-of select="b:assert(@type, @key, 'year',        $year  )"/>
			</xsl:when>

			<xsl:when test="@type = 'unpublished'">
				<xsl:value-of select="b:assert(@type, @key, 'author', $author)"/>
				<xsl:value-of select="b:assert(@type, @key, 'title',  $title )"/>
				<xsl:value-of select="b:assert(@type, @key, 'note',   $note  )"/>
			</xsl:when>

			<xsl:otherwise>
				<xsl:message terminate="yes">
					<xsl:text>unimplemented type: @</xsl:text>
					<xsl:value-of select="@type"/>
					<xsl:text>&#10;</xsl:text>
				</xsl:message>
			</xsl:otherwise>
		</xsl:choose>

		<!-- TODO: bibtex as @data -->
		<dt id="{@key}">
			<xsl:choose>
				<xsl:when test="$key">
					<xsl:apply-templates select="$key"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="str:tokenize($author/b:value[1], ',')[1]"/>
					<xsl:value-of select="substring($year/b:value[1], string-length($year/b:value[1]) + 1 - 2, 2)"/>
					<!-- TODO: uniqueify -->
					<!-- TODO: option to generate as <ul> instead, and number by css -->
				</xsl:otherwise>
			</xsl:choose>
		</dt>
		<dd class="{@type}">
			<xsl:apply-templates select="b:field/b:value" mode="comment"/>

			<xsl:apply-templates select="common:node-set($author)"/>

			<xsl:if test="$author and $title">
				<xsl:text> </xsl:text>
			</xsl:if>
			<xsl:apply-templates select="common:node-set($title)"/>

			<xsl:if test="@type = 'mastersthesis'">
				<xsl:text> </xsl:text>
				<xsl:text>Master&#8217;s thesis.</xsl:text>
			</xsl:if>

			<xsl:if test="@type = 'phdthesis'">
				<abbr>PhD</abbr> <!-- TODO: or DPhil -->
				<xsl:text> thesis.</xsl:text>
			</xsl:if>

			<xsl:if test="$journal">
				<xsl:text> </xsl:text>
				<xsl:apply-templates select="common:node-set($journal)"/>
			</xsl:if>

			<xsl:if test="$booktitle">
				<xsl:text> </xsl:text>
				<xsl:apply-templates select="common:node-set($booktitle)"/>
			</xsl:if>

			<xsl:if test="$edition">
				<xsl:text> </xsl:text>
				<xsl:apply-templates select="common:node-set($edition)"/>
			</xsl:if>

			<xsl:if test="$series">
				<xsl:text> </xsl:text>
				<xsl:apply-templates select="common:node-set($series)"/>
			</xsl:if>

			<xsl:if test="@type = 'techreport'">
				<xsl:if test="$number">
						<xsl:if test="not($series)">
							<xsl:text>. </xsl:text>
							<xsl:text>Report</xsl:text>
						</xsl:if>

					<xsl:text> </xsl:text>
					<xsl:apply-templates select="common:node-set($number)"/>
					<xsl:text>,</xsl:text>
				</xsl:if>
			</xsl:if>

			<xsl:if test="$volume or $pages or $chapter">
				<xsl:text> </xsl:text>
				<xsl:call-template name="location">
					<xsl:with-param name="volume"  select="$volume"/>
					<xsl:with-param name="number"  select="$number"/> <!-- XXX: repetition from techreport -->
					<xsl:with-param name="pages"   select="$pages"/>
					<xsl:with-param name="chapter" select="$chapter"/>
				</xsl:call-template>
			</xsl:if>

			<xsl:if test="$month or $year">
				<xsl:text> </xsl:text>
				<xsl:call-template name="monthyear">
					<xsl:with-param name="month" select="$month"/>
					<xsl:with-param name="year"  select="$year"/>
				</xsl:call-template>
				<xsl:text>.</xsl:text>
			</xsl:if>

			<xsl:if test="$editor">
				<xsl:text> Ed.&#xA0;</xsl:text>
				<xsl:apply-templates select="common:node-set($editor)"/>
			</xsl:if>

			<xsl:if test="$school">
				<xsl:text> </xsl:text>
				<xsl:apply-templates select="common:node-set($school)"/>
			</xsl:if>

			<xsl:if test="$organization">
				<xsl:text> </xsl:text>
				<xsl:apply-templates select="common:node-set($organization)"/>
			</xsl:if>

			<xsl:if test="$institution">
				<xsl:text> </xsl:text>
				<xsl:apply-templates select="common:node-set($institution)"/>
			</xsl:if>

			<xsl:if test="$publisher">
				<xsl:text> </xsl:text>
				<xsl:apply-templates select="common:node-set($publisher)"/>
			</xsl:if>

			<xsl:if test="$address">
				<xsl:text> </xsl:text>
				<xsl:apply-templates select="common:node-set($address)"/>
				<xsl:text>, </xsl:text>
			</xsl:if>

			<xsl:if test="$howpublished">
				<xsl:text> </xsl:text>
				<xsl:apply-templates select="common:node-set($howpublished)"/>
			</xsl:if>

			<xsl:text>.</xsl:text>

			<xsl:if test="$others">
				<xsl:text> </xsl:text>
				<xsl:apply-templates select="common:node-set($others)"/>	<!-- TODO: space before each -->
			</xsl:if>

			<xsl:if test="$note">
				<xsl:text> </xsl:text>
				<xsl:apply-templates select="common:node-set($note)"/>
			</xsl:if>

			<xsl:if test="$issn or $isbn">
				<dl class="isxn">
					<xsl:if test="$issn">
						<dt>
							<xsl:text>ISSN</xsl:text>
						</dt>
						<dd>
							<xsl:apply-templates select="common:node-set($issn)"/>
						</dd>
					</xsl:if>

					<xsl:if test="$isbn">
						<dt>
							<xsl:text>ISBN</xsl:text>
						</dt>
						<dd>
							<xsl:apply-templates select="common:node-set($isbn)"/> <!-- TODO: br between each -->
						</dd>
					</xsl:if>
				</dl>
			</xsl:if>

			<!-- TODO: param for local website base -->
			<xsl:if test="not(starts-with($url/b:value, 'http://www.tendra.org/'))">
				<xsl:apply-templates select="common:node-set($url)"/>
			</xsl:if>

			<ul class="otherformats">
				<li>
					<!-- TODO: a rel="something" -->
					<!-- TODO: variable for base path -->
					<a href="{concat('/', 'bib')}" data-type="bib">
						<abbr class="tex">
							<xsl:text>Bɪʙ</xsl:text>
							<xsl:text>T</xsl:text>
							<sub>
								<xsl:text>e</xsl:text>
							</sub>
							<xsl:text>X</xsl:text>
						</abbr>
					</a>
				</li>

				<xsl:apply-templates select="common:node-set($file)" mode="otherformats"/>
				<xsl:apply-templates select="common:node-set($doi)"  mode="otherformats"/>
				<xsl:if test="starts-with($url/b:value, 'http://www.tendra.org/')">
					<xsl:apply-templates select="common:node-set($url)" mode="otherformats"/>
				</xsl:if>

				<!-- TODO: no need, but make it an option:
					<xsl:if test="not($file)">
						<li class="na">
							<xsl:text>(no local copy)</xsl:text>
						</li>
					</xsl:if>
				-->
			</ul>
		</dd>
	</xsl:template>

	<xsl:template match="/b:bibt">
		<html>
			<body>
				<dl class="bibliography">
						<xsl:apply-templates select="b:entry"/>
				</dl>
			</body>
		</html>
	</xsl:template>

</xsl:stylesheet>

